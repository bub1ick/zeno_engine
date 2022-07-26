#include <zeno.hpp>

#ifdef _WIN32
int32_t APIENTRY WinMain(
    _In_ HINSTANCE instance, _In_opt_ HINSTANCE prev_instance, _In_ char* command_line, _In_ int32_t show_command
)
{
    zeno::application_t application("test");
    application.run();

    return 0;
}
#endif
