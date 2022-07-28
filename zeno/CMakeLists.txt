# The engine CMakeLists.txt file. Defines build rules for all engine modules and libs.

add_compile_definitions(ZENO_LIB)

add_subdirectory(zenodx11)

add_subdirectory(zenocore)