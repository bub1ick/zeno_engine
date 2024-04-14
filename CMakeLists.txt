cmake_minimum_required(VERSION 3.22)

## Project declaration
project(
    dx11renderer
    VERSION 0.1.0
    DESCRIPTION "\"zeno_engine\" is a collection of tools and libraries to create interactive expreriences."
    LANGUAGES CXX
)


## Language settings:
# Set C++ standard.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)


if(MSVC)
    # Enable some of the standard C++ features, such as literals for logical operators.
    add_compile_options(/permissive-)

    # Enable utf-8 source and runtime(?) encoding on msvc. When using clang on Windows it's the default.
    add_compile_options(/utf-8)
endif()


## Build settings:
# By default, use dynamic libs for modules, unless it's not a module
set(BUILD_SHARED_LIBS ON)
set(CMAKE_DEBUG_POSTFIX "_d")

if(NOT ${WIN32})
    message(FATAL_ERROR "Currently support only for Windows is planned.")
endif()
add_compile_definitions("ZENO_WINDOWS")

set(ZENO_THIRD_PARTY_LIB_PATH "${CMAKE_HOME_DIRECTORY}/third_party/lib")
set(ZENO_THIRD_PARTY_INCLUDE_PATH "${CMAKE_HOME_DIRECTORY}/third_party/include")

link_directories(AFTER ${ZENO_THIRD_PARTY_LIB_PATH})

include_directories(AFTER ${ZENO_THIRD_PARTY_INCLUDE_PATH})

## Add directories:

# Declare renderer executable utilizing the zeno engine.
add_executable(application WIN32)

target_link_libraries(
    application
    PUBLIC
    "DXGI.lib"
    "D3D11.lib"
    "D3DCompiler.lib"
)

# Add .cpp files for the executable
target_sources(
    application
    PRIVATE
    "sources/window.hpp"
    "sources/window.cpp"
    "sources/renderer.hpp"
    "sources/renderer.cpp"
    "sources/dxgi.hpp"
    "sources/dxgi.cpp"
    "sources/dx11.hpp"
    "sources/dx11.cpp"
    "sources/exceptions.hpp"
    "sources/exceptions.cpp"
    "sources/engine.hpp"
    "sources/engine.cpp"
    "sources/gltf_loader.hpp"
    "sources/gltf_loader.cpp"
    "sources/zeno.hpp"
    "sources/zeno_defines.hpp"
    "sources/main.cpp"
    "../resources/application.manifest"
)


## Build script for the zeno engine.
#add_subdirectory(zeno)
#
## At last build the sample to test the engine.
#if(${ZENO_BUILD_SAMPLE})
#    add_subdirectory(sample)
#    add_dependencies(sample zeno)
#    add_dependencies(sample zenolib)
#    add_dependencies(zeno zenogfx)
#endif()
#
#install(
#    TARGETS zeno zenogfx zenolib
#    CONFIGURATIONS Release
#    ARCHIVE
#    DESTINATION "${CMAKE_HOME_DIRECTORY}/lib"
#)
#install(
#    TARGETS zeno zenogfx zenolib
#    CONFIGURATIONS Debug
#    ARCHIVE
#    DESTINATION "${CMAKE_HOME_DIRECTORY}/lib"
#)
#install(
#    TARGETS zeno zenogfx
#    CONFIGURATIONS Debug
#    RUNTIME
#    DESTINATION "${CMAKE_HOME_DIRECTORY}/bin"
#)
#install(
#    TARGETS zeno zenogfx
#    CONFIGURATIONS Release
#    RUNTIME
#    DESTINATION "${CMAKE_HOME_DIRECTORY}/bin"
#)
#
#install(
#    TARGETS sample
#    CONFIGURATIONS Release
#    RUNTIME
#    DESTINATION "${CMAKE_HOME_DIRECTORY}/bin"
#)
#install(
#    TARGETS sample
#    CONFIGURATIONS Debug
#    RUNTIME
#    DESTINATION "${CMAKE_HOME_DIRECTORY}/bin"
#)

