

#include <cstdio>
#include <string>

#include "rs_manager.hpp"
#include "rs_node.hpp"

const char *PATH = "foo";
const size_t SIZE = 128;

int main()
{
    // Get a manager for the mapped region
    RSManager m(PATH, SIZE);
    if (!m.is_okay())
    {
        fprintf(stderr, "m is not okay\n");
    }

    auto n = m.AllocNode();
}