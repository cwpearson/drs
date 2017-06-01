#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <cassert>
#include <chrono>
#include <thread>

int createOrResizeFile(const std::string &path, const size_t size) {
  int fd = open(path.c_str(), O_RDWR);
  if (fd == -1 && errno == ENOENT) // file does not exist. create
  {
    fd = open(path.c_str(), O_RDWR | O_CREAT, 0666);
    if (fd != -1)
    {
      ftruncate(fd, size); // resize
    }
  }
  return fd;
}

char *mapFile(const std::string &path, const size_t size) {

  // Open file
  int fd = createOrResizeFile(path, size);
  if (-1 == fd)
  {
    fprintf(stderr, "create/resize failed: %s\n", strerror(errno));
    return nullptr;
  }

  // mmap
  // const int pagesize = getpagesize();
  const int offset = 0;
  char *data = (char *)mmap((caddr_t)0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, offset);
  if ((intptr_t)data == -1)
  {
    fprintf(stderr, "mmap failed: %s\n", strerror(errno));
    return nullptr;
  }
  return data;
}

class DrsAtomicInt32 {
 private:
  volatile char *region_;
  size_t offset_;
 public:
  DrsAtomicInt32(char *reg, size_t off, int32_t val) : region_(reg), offset_(off) {
    auto ptr = reinterpret_cast<volatile int32_t*>(region_ + offset_);
    *ptr = val;
  }
  int32_t increment(int32_t inc) {
    auto ptr = reinterpret_cast<volatile int32_t*>(region_ + offset_);
    return __sync_fetch_and_add(ptr, inc);
  }
  int32_t fetch() const {
    auto ptr = reinterpret_cast<volatile int32_t*>(region_ + offset_);
    return *ptr;
  }
  DrsAtomicInt32 &operator=(const int32_t i) {
    auto ptr = reinterpret_cast<volatile int32_t*>(region_ + offset_);
    *ptr = i;
    return *this;
  }
};

class DrsVectorDouble {
 private:
  volatile char *region_;  // base of the mmap region
  size_t offset_; // beginning of this object in that region
  size_t capacity_;
 public:
  DrsVectorDouble(char *region, size_t offset, size_t capacity) 
    : region_(region), offset_(offset), capacity_(capacity) {}

  volatile double &operator[](const size_t i) {
    return *(reinterpret_cast<volatile double*>(region_ + offset_) + i);
  }

  const double &operator[](const size_t i) const {
    return this->operator[](i);
  }

  size_t size() const { return capacity_; }
};

int main(void) {
  const size_t VEC_EXTENT = 1024 * 1024;
  const size_t REGION_BYTES = 3 * VEC_EXTENT * sizeof(double) + 3 * sizeof(int32_t);
  auto region = mapFile("foo", REGION_BYTES);
  if (nullptr == region) {
    fprintf(stderr, "No luck\n");
    return EXIT_FAILURE;
  }

  size_t offset = 0;
  DrsAtomicInt32 todo(region, offset, 0); // start of unadded region
  offset += sizeof(int32_t);
  DrsAtomicInt32 complete(region, offset, 0); // is python done
  offset += sizeof(int32_t);
  DrsAtomicInt32 go(region, offset, 0); // python may start
  offset += sizeof(int32_t);
  DrsVectorDouble a(region, offset, VEC_EXTENT);
  offset += sizeof(double) * a.size();
  DrsVectorDouble b(region, offset, VEC_EXTENT);
  offset += sizeof(double) * b.size();
  DrsVectorDouble c(region, offset, VEC_EXTENT);
  offset += sizeof(double) * c.size();

// Initialize the vectors
  for (size_t i = 0; i < a.size(); ++i) {
    a[i] = i;
  }
  for (size_t i = 0; i < b.size(); ++i) {
    b[i] = i;
  }
  
  go = 1; // tell python to go

  size_t my_work = 0; // how many numbers C++ added
  const int BS = 10;
  int start;
  while ((start = todo.increment(BS)) < c.size()) {
    // printf("C++ claimed %d-%d\n", start, start + BS - 1);
    for (int i = start; i < start + BS && i < c.size(); ++i) {
      c[i] = a[i] + b[i];
    }
    my_work += BS;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }

  printf("%lu\n", my_work);
  printf("Waiting for Python to finish...\n");
  while (!complete.fetch()) {};

  printf("checking...\n");
  for (size_t i = 0; i < c.size(); ++i) {
    if (c[i] != a[i] + b[i]) {
      printf("%lu: %f != %f + %f\n", i, c[i], a[i], b[i]);
    }
  }
  printf("...done!\n");
}
