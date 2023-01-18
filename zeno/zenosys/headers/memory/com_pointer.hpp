#pragma once

#include "com_pointer/com_deleter.hpp"

namespace zeno::sys
{
template <typename ComInterface>
using unique_com_t = std::unique_ptr<ComInterface, com_deleter_t<ComInterface>>;

template <typename ComInterface>
using shared_com_t = std::shared_ptr<ComInterface>;

}