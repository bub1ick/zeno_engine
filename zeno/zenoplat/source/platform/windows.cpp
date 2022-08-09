#include <zeno.hpp>

namespace zeno
{
void platform_loop(const std::function<void()>& engine_loop)
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
}  //  namespace zeno