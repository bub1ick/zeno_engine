#pragma once

//  standard libs
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
#include <typeinfo>
#include <chrono>

#if defined(ZENO_WINDOWS)
#    define NOGDI
#    define NOKERNEL
#    define NOMINMAX

//  HACK: Use this to let the windows headers define the GUIDs of interfaces.
//  We want such behavior because using compiler intrinsincs is kinda bad imo.
#    define INITGUID

//  namespace winapi
//{
#    include <windows.h>

#    include <d3d11_4.h>
#    include <d3dcompiler.h>
#    include <dxgi1_6.h>
#    include <DirectXMath.h>
#    include <DirectXColors.h>
#    include <DirectXPackedVector.h>
//}
#endif


//  macros and various preprocessor definitions
#include "zeno_defines.hpp"

//  third party libs:
//  .gltf 3D file format loader
#include <tinygltf/tiny_gltf.h>

//  engine libs
#include "zenoutils/zenoutils.hpp"
#include "zenosys/zenosys.hpp"

//  engine modules
#include "zenogfx/zenogfx.hpp"
#include "zeno/zeno.hpp"
