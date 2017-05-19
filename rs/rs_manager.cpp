#include <unistd.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>

#include "rs_manager.hpp"

int createOrOpenFile(const std::string &path, const size_t size)
{
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

RSManager::RSManager(const std::string &path, const size_t size) : fd_(-1), size_(0)
{
  // Open file
  fd_ = createOrOpenFile(path, size);
  if (-1 == fd_)
  {
    fprintf(stderr, "create/open failed: %s\n", strerror(errno));
  }

  // mmap
  // const int pagesize = getpagesize();
  const int offset = 0;
  data_ = (char *)mmap((caddr_t)0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd_, offset);
  if ((intptr_t)data_ == -1)
  {
    fprintf(stderr, "mmap failed: %s\n", strerror(errno));
  }
  free_ = data_;
  size_ = size;
}
