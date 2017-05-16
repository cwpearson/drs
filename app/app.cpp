#include "../include/drs.hpp"

#include "node_drs.hpp"

int main()
{
    auto root_drs = DRS::Allocate<Node>();
    auto left_native = new Node;
    auto left_drs = DRS::Share(left_native);
    auto right_drs = DRS::Allocate<Node>();

    root_drs.set_field("left_", left_drs);
    root_drs.set_field("right_", right_drs);
}