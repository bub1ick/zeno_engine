#include <zeno.hpp>

namespace zeno
{

engine_t::engine_t(const char* window_name, int32_t pos_x, int32_t pos_y, int32_t size_x, int32_t size_y)
    : _renderer(window_name, pos_x, pos_y, size_x, size_y)
{
}

void engine_t::run()
{
    _renderer.initialize();

    MSG message;
    while (true)
    {
        if (GetMessageA(&message, _renderer.get_window().get_window_handle(), 0, 0) != 0)
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
}

}  //  namespace zeno
