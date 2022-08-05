#include <zeno.hpp>

namespace zeno
{

engine_t::engine_t(const char* window_name, int32_t pos_x, int32_t pos_y, int32_t size_x, int32_t size_y)
{
    try
    {
        //  Try getting the application instance
        HINSTANCE instance = GetModuleHandleA(nullptr);
        if (!instance)
        {
            throw std::runtime_error(
                "Could not get .exe instance handle. To avoid this issue, please use the full constructor."
            );
        }

        _renderer = new renderer_t(window_name, instance, pos_x, pos_y, size_x, size_y);
    }
    catch (std::exception& exception)
    {
        std::cout << exception.what();
    }
}

engine_t::~engine_t()
{
    delete _renderer;
}

void engine_t::run()
{

    _renderer->initialize();

    MSG message;
    while (true)
    {
        if (GetMessageA(&message, _renderer->get_window()->get_window_handle(), 0, 0) != 0)
        {
            TranslateMessage(&message);
            DispatchMessageA(&message);
        }
    }
}

}  //  namespace zeno
