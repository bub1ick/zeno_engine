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
#include <memory>

#if defined(ZENO_WINDOWS)
#    define NOGDI
#    define NOKERNEL
#    define NOMINMAX

//  HACK: Use this to let the windows headers define the GUIDs of interfaces.
// We want such behavior because using compiler intrinsincs is kinda bad imo.
#    define INITGUID

#    include <windows.h>

#    include <d3d11_4.h>
#    include <dxgi1_6.h>
#endif

#include "zeno_defines.hpp"

//  engine libs
#include "zenoutils/zenoutils.hpp"
#include "zenosys/zenosys.hpp"
#include "zenodx11/zenodx11.hpp"
#include "zeno/zeno.hpp"
