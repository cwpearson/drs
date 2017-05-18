#ifndef RS_MANAGER_HPP
#define RS_MANAGER_HPP

#include <cstdlib>
#include <string>

#include "rs_object.hpp"
#include "rs_node.hpp"

class RSManager
{
  private:
    int fd_;     // file descriptor to mapped region
    void *data_; // pointer to mapped region

  public:
    RSManager() : fd_(-1), data_(nullptr) {}
    RSManager(const std::string &path, const size_t size);

    bool is_okay()
    {
        return (-1 != fd_ && nullptr != data_);
    }

    template <typename T>
    T CreateShared();

    Node AllocNode()
    {
        auto loc = reinterpret_cast<char *>(data_);
        auto n = Node(this, loc, 5);
        return n;
    }
};

#endif