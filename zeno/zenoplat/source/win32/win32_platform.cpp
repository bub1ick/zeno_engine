#include <zeno.hpp>

namespace zeno::plat
{
window_t* create_platform_window(
    const char* in_window_title, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y
)
{
    HINSTANCE instance = GetModuleHandleA(0);

    return new win32::win32_window_t(in_window_title, in_pos_x, in_pos_y, in_size_x, in_size_y, instance);
}

void platform_loop(const std::function<void()>& in_engine_loop)
{
    MSG message;
    while (true)
    {
        if (GetMessageA(&message, nullptr, 0, 0) != 0)
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
}
}  //  namespace zeno::plat