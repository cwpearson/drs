#ifndef RS_OBJECT_HPP
#define RS_OBJECT_HPP

#include <cstdlib>
#include <cstring>

class RSManager;
class RSRefBase;

class RSObject
{
  friend RSManager;

protected:
  static constexpr size_t underlying_size_ = 0;
  char *underlying_object_;
  RSManager *manager_;

public:
  RSObject(RSManager *man);
  virtual void realize(char *dst) = 0;
  virtual RSRefBase Ref();
};

#endif
