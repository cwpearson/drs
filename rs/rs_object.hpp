#ifndef RS_OBJECT_HPP
#define RS_OBJECT_HPP

#include <cstdlib>
#include <cstring>

class RSManager;

class RSObject
{
  friend RSManager;

protected:
  char *underlying_object_;
  RSManager *manager_;
  static constexpr size_t underlying_size_ = 0;

public:
  RSObject(RSManager *man, char *obj) : manager_(man), underlying_object_(obj) {}
};

// Represents a reference to another object in the mapped region
template <typename T>
class RSRef : public RSObject
{

public:
  static constexpr size_t underlying_size_ = sizeof(size_t);
  RSRef(RSManager *man, char *obj, size_t dst) : RSObject(man, obj), dst_(dst) {}
  RSRef(RSManager *man, char *obj) : RSRef(man, obj, -1) {}
  RSRef() : RSRef(nullptr, nullptr, -1) {}

  size_t dst_; // this ref points to offset dst_ in the mapped region
};

#endif
