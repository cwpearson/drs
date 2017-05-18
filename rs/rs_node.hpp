#ifndef NODE_RS_HPP
#define NODE_RS_HPP

#include "rs_object.hpp"
#include "rs_manager.hpp"

class Node : public RSObject
{
private:
  RSRef<Node> left_;

private:
  static constexpr size_t underlying_size_ = RSRef<Node>::underlying_size_ + sizeof(double);

public:
  Node(RSManager *man, char *obj, double d) : RSObject(man, obj)
  {
    std::memcpy(obj + RSRef<Node>::underlying_size_, &d, sizeof(double)); // store the double
  }
  Node(RSManager *man, char *obj) : Node(man, obj, -3) {}
  Node *left();
  double &value();
};

#endif
