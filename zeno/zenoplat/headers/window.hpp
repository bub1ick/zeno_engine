#pragma once

#include "window/window_dimenstions.hpp"

namespace zeno
{
namespace plat
{
    class ZENO_API window_t
    {
    protected:

        window_dimensions_t m_window_dimenstions;

        const char*         m_title;
    };
}  //  namespace plat
}  //  namespace zeno

#undef ZENO_LIB