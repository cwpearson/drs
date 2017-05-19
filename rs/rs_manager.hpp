#ifndef RS_MANAGER_HPP
#define RS_MANAGER_HPP

#include <cstdlib>
#include <string>

#include "rs_ref.hpp"

class RSManager
{
  private:
    int fd_;      // file descriptor to mapped region
    char *data_;  // pointer to mapped region
    char *free_;  // pointer to free space
    size_t size_; // size of mapped region

  public:
    RSManager() : fd_(-1), data_(nullptr), free_(data_), size_(0) {}
    RSManager(const std::string &path, const size_t size);

    bool is_okay()
    {
        return (-1 != fd_ && nullptr != data_);
    }

    char *Alloc(size_t nbytes)
    {
        if (free_ - data_ < size_)
        {
            auto loc = free_;
            free_ += nbytes;
            return loc;
        }
        else
        {
            fprintf(stderr, "region:     %p - %p\n", data_, data_ + size_);
            fprintf(stderr, "free start: %p\n", free_);
            fprintf(stderr, "OOM\n");
            return nullptr;
        }
    }

    template <typename T>
    RSRef<T> get_ref(const T &t)
    {
        return RSRef<T>(this, t.underlying_object_ - data_);
    }
};

#endif