#! /bin/bash

text="#ifndef DRS_NODE_HPP
#define DRS_NODE_HPP

#include <cstdint>

struct Node
{
  Node *left_;
  Node *right_;
  int64_t data_;
};

#endif"

path="`pwd`/node_drs.hpp"
echo "gen $path..."
echo "$text" > "$path"

text="class Node:
    data_ = None
    left_ = None
    right_ = None"

path="`pwd`/node_drs.py"
echo "gen $path..."
echo "$text" > "$path"