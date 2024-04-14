# Declare renderer executable utilizing the zeno engine.
add_executable(renderer WIN32)

target_link_libraries(
    renderer
    PUBLIC
    zeno
)

# Add include directory to be able to use engine's primary include file (zeno.hpp)
target_include_directories(
    renderer
    PRIVATE
    "${PROJECT_SOURCE_DIR}/zeno/"
)

# Add .cpp files for the executable
target_sources(
    renderer
    PRIVATE
    "window.hpp"
    "window.cpp"
    "renderer.hpp"
    "renderer.cpp"
    "dxgi.hpp"
    "dxgi.cpp"
    "dx11.hpp"
    "dx11.cpp"
    "exceptions.hpp"
    "exceptions.cpp"
    "engine.hpp"
    "engine.cpp"
    "gltf_loader.hpp"
    "gltf_loader.cpp"
    "zeno.hpp"
    "zeno_defines.hpp"
    "main.cpp"
    "../resources/application.manifest"
)

