#include "drs.hpp"

#include "drs_node_cpp.hpp"

int main()
{
    Node *root_native = new Node(0);
    root_native->left_ = new Node(1);
    root_native->right_ = new Node(2);

    auto root_drs = DRS::Allocate<Node>();
}