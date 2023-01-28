# The engine CMakeLists.txt file. Defines build rules for all engine modules and libs.
add_subdirectory(zenoutils)
add_subdirectory(zenosys)
add_subdirectory(zenogfx)
add_subdirectory(zeno)

# apply appropriate build steps and rules for resources used by the engine
add_subdirectory(resources)
