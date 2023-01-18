#pragma once

namespace zeno::sys
{
template <typename ComType>
class com_deleter_t
{
public:
    void operator()(ComType* out_pointer)
    {
        if (out_pointer)
        {
            out_pointer->Release();
            out_pointer = nullptr;
        }
    }
};
}  //  namespace zeno::sys