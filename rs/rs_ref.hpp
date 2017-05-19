#ifndef RS_REF_HPP
#define RS_REF_HPP

#include <cstdlib>
#include <cstring>

#include "rs_object.hpp"

class RSRefBase : public RSObject
{
protected:
  size_t dst_; // this ref points to offset dst_ in the mapped region
public:
  static constexpr size_t underlying_size_ = sizeof(size_t);
  RSRefBase(RSManager *man, size_t dst);
  RSRefBase(RSManager *man);
  RSRefBase();

  void realize(char *dst);
};

// Represents a reference to another value in the mapped region
template <typename T>
class RSRef : public RSRefBase
{
public:
  RSRef() : RSRefBase() {}
  RSRef(RSManager *man, size_t dst) : RSRefBase(man, dst) {}
};

#endif
