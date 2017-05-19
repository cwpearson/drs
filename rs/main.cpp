#include <cstdio>
#include <string>

#include "rs_manager.hpp"
#include "rs_node.hpp"
#include "rs_ref.hpp"

const char *PATH = "foo";
const size_t SIZE = 1024 * 1024;

int main()
{
    // Get a manager for the mapped region
    RSManager m(PATH, SIZE);
    if (!m.is_okay())
    {
        fprintf(stderr, "m is not okay\n");
    }

    auto n1 = Node::CreateShared(m, 1);
    auto n2 = Node::CreateShared(m, 2, n1.Ref());
    auto n3 = Node::CreateShared(m, 3, n2.Ref());
}