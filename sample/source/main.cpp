

#include <zeno.hpp>

// #if defined(TINYGLTF_IMPLEMENTATION)
// #    error "everything is defined correctly
// #endif

#ifdef ZENO_WINDOWS

int32_t APIENTRY WinMain(_In_ HINSTANCE instance, _In_opt_ HINSTANCE prev_instance, _In_ char* command_line, _In_ int32_t show_command)
{
    zeno::engine_t engine("test");
    return engine.run();
}

#elif

int32_t main(int32_t agrc, char** argv)
{
    zeno::engine_t engine("test");
    return engine.run();
}

#endif
