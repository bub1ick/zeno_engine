#pragma once

//  standard lib
#include <cstdint>
#include <cstring>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <functional>
#include <fstream>
#include <sstream>

#if defined(ZENO_WINDOWS)
#    define NOGDI
#    define NOKERNEL
#    define NOMINMAX

#    include <windows.h>

#    include <d3d11_4.h>
#    include <dxgi1_6.h>
#endif

#include "zeno_defines.hpp"

//  engine libs
#include "zenocore/zenocore.hpp"
