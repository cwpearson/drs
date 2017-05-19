#include "rs_object.hpp"
#include "rs_manager.hpp"
#include "rs_ref.hpp"

RSObject::RSObject(RSManager *man) : manager_(man), underlying_object_(nullptr) {}

RSRefBase RSObject::Ref()
{
  return manager_->get_ref(*this);
}
