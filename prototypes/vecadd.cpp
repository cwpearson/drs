#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <string>

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
  char *region_;
  size_t offset_;
 public:
  DrsAtomicInt32(char *reg, size_t off, int32_t val) : region_(reg), offset_(off) {
    auto ptr = reinterpret_cast<int32_t*>(region_ + offset_);
    *ptr = val;
  }
  int32_t increment(int32_t inc) {
    int32_t *ptr = reinterpret_cast<int32_t*>(region_ + offset_);
    return __sync_fetch_and_add(ptr, inc);
  }
};

class DrsVectorDouble {
 private:
  char *region_;  // base of the mmap region
  size_t offset_; // beginning of this object in that region
  size_t capacity_;
 public:
  DrsVectorDouble(char *region, size_t offset, size_t capacity) 
    : region_(region), offset_(offset), capacity_(capacity) {}

  double &operator[](const size_t i) {
    return *(reinterpret_cast<double*>(region_ + offset_) + i);
  }

  const double &operator[](const size_t i) const {
    return this->operator[](i);
  }

  size_t size() const { return capacity_; }
};

int main(void) {
  const size_t G = 1024 * 1024 * 1024;
  auto region = mapFile("foo", G);
  if (nullptr == region) {
    fprintf(stderr, "No luck\n");
    return EXIT_FAILURE;
  }

  size_t offset = 0;
  DrsAtomicInt32 i32(region, 0, 0);
  offset += sizeof(int32_t);
  DrsVectorDouble a(region, offset, 1024*1024 / sizeof(double));
  offset += sizeof(double) * a.size();
  DrsVectorDouble b(region, offset, 1024*1024 / sizeof(double));
  offset += sizeof(double) * b.size();
  DrsVectorDouble c(region, offset, 1024*1024 / sizeof(double));
  offset += sizeof(double) * c.size();
 
  for (size_t i = 0; i < a.size(); ++i) {
    a[i] = i;
  }
  for (size_t i = 0; i < b.size(); ++i) {
    b[i] = i;
  }
  
}
