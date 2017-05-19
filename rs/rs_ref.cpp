#include <cstdlib>
#include <cstring>

#include "rs_manager.hpp"

RSRefBase::RSRefBase(RSManager *man, size_t dst) : RSObject(man), dst_(dst) {}
RSRefBase::RSRefBase(RSManager *man) : RSObject(man) {}
RSRefBase::RSRefBase() : RSRefBase(nullptr) {}

void RSRefBase::realize(char *dst)
{
  std::memcpy(dst, &dst_, sizeof(dst_));
}
