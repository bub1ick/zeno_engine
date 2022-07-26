#include <zeno.hpp>

namespace zeno
{
window_t::window_t(
    const char* window_name, int32_t pos_x, int32_t pos_y, int32_t size_x, int32_t size_y, HINSTANCE instance,
    int32_t show_command
)
    : _title(window_name),
      _pos_x(pos_x),
      _pos_y(pos_y),
      _size_x(size_x),
      _size_y(size_y)
{
    set_up_window_class(instance);
    create_window(show_command);
}

HWND window_t::get_window_handle() const
{
    return _handle;
}

ATOM window_t::get_class_atom() const
{
    return _class_atom;
}

void window_t::set_up_window_class(HINSTANCE instance)
{
    _class.cbSize        = sizeof(WNDCLASSEXA);
    _class.style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    _class.lpfnWndProc   = _window_procedure;
    _class.cbClsExtra    = 0;
    _class.cbWndExtra    = sizeof(this);
    _class.hInstance     = instance;
    _class.hIcon         = nullptr;
    _class.hCursor       = LoadCursor(instance, IDC_ARROW);
    _class.hbrBackground = nullptr;
    _class.lpszMenuName  = nullptr;
    _class.lpszClassName = "zeno application";
    _class.hIconSm       = nullptr;

    _class_atom          = RegisterClassExA(&_class);
}

void window_t::create_window(int32_t show_command)
{
    _handle = CreateWindowExA(
        WS_EX_OVERLAPPEDWINDOW, MAKEINTATOM(_class_atom), _title, WS_OVERLAPPEDWINDOW, _pos_x, _pos_y, _size_x, _size_y,
        nullptr, nullptr, _class.hInstance, this
    );

    ShowWindow(_handle, show_command);
    UpdateWindow(_handle);
}

LRESULT window_t::_window_procedure(HWND window, uint32_t message_id, WPARAM w_param, LPARAM l_param)
{
    switch (message_id)
    {
        case WM_CREATE:
        {
            CREATESTRUCTA* create_struct = reinterpret_cast<CREATESTRUCTA*>(l_param);

            SetWindowLongPtrA(window, GWLP_USERDATA, (LONG_PTR) create_struct->lpCreateParams);

            return 0;
        }
        case WM_CLOSE:
        {
            window_t* this_window = reinterpret_cast<window_t*>(GetWindowLongPtrA(window, GWLP_USERDATA));

            return 0;
        }
        case WM_DESTROY:
        {
            return 0;
        }
        default:
        {
            return DefWindowProcA(window, message_id, w_param, l_param);
        }
    }
}
}  //  namespace zeno
