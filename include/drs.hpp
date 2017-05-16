#ifndef DRS_HPP
#define DRS_HPP

#include <string>
#include <map>
#include <cstdlib>
#include <cstring>
#include <vector>

namespace DRS
{

class Value
{
  private:
    std::vector<char> data;
};

class ObjectRef : public Value
{

  public:
    void set_field(const std::string &field_name, const Value &val)
    {
    }

    Value get_field(const std::string &field_name)
    {
        Value v;
        return v;
    }
};

// create a new shared object
template <typename T>
ObjectRef Allocate() { return ObjectRef(); }

// make an existing object visible
template <typename T>
ObjectRef Share(const T *) { return ObjectRef(); }
}

#endif