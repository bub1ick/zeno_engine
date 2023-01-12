#pragma once

#include "com_pointer/com_deleter.hpp"

namespace zeno::utils
{
template <typename ComInterface>
using com_unique_ptr = std::unique_ptr<ComInterface, com_deleter_t<ComInterface>>;
}