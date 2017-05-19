#ifndef RS_NODE_HPP
#define RS_NODE_HPP

#include "rs_object.hpp"
#include "rs_ref.hpp"
#include "rs_manager.hpp"

class Node : public RSObject
{
  friend RSManager;

private:
  RSRef<Node> left_;
  static constexpr size_t underlying_size_ = RSRef<Node>::underlying_size_ + sizeof(double);

public:
  static Node CreateShared(RSManager &manager, double d);
  static Node CreateShared(RSManager &manager, double d, RSRef<Node> left);

  RSRef<Node> Ref() const
  {
    return manager_->get_ref(*this);
  }

  void Set_left(RSRef<Node> &r);
  void Set_value(double d);

  Node(RSManager *man) : RSObject(man) {}

  void realize(char *dst)
  {
    underlying_object_ = dst;
  }

  Node *left();
  double &value();
};

#endif
