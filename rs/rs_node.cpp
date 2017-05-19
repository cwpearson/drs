#include "rs_node.hpp"

Node Node::CreateShared(RSManager &manager, double d)
{
  // Get space from the manager
  auto start = manager.Alloc(underlying_size_);
  auto n = Node(&manager);
  n.realize(start);

  // store the double
  n.Set_value(d);

  return n;
}

Node Node::CreateShared(RSManager &manager, double d, RSRef<Node> left)
{
  // Get space from the manager
  auto n = Node::CreateShared(manager, d);

  // Store the reference
  n.Set_left(left);

  return n;
}

void Node::Set_left(RSRef<Node> &r)
{
  r.realize(underlying_object_);
}

void Node::Set_value(double d)
{
  std::memcpy(underlying_object_ + RSRef<Node>::underlying_size_, &d, sizeof(double)); // store the double
}