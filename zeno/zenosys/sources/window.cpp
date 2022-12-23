#include <zeno.hpp>

namespace zeno::sys
{
window_t::window_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y)
{
    //  passing 0 to GetModuleHandle returns the calling .exe instance
    m_application_instance = GetModuleHandleA(0);

    WNDCLASSEXA window_class {};
    window_class.cbSize        = sizeof(WNDCLASSEXA);
    window_class.style         = 0;
    window_class.lpfnWndProc   = window_procedure;
    window_class.cbClsExtra    = 0;
    window_class.cbWndExtra    = 0;
    window_class.hInstance     = m_application_instance;
    window_class.hIcon         = nullptr;
    window_class.hCursor       = nullptr;
    window_class.hbrBackground = nullptr;
    window_class.lpszMenuName  = nullptr;
    window_class.lpszClassName = "ZenoApplication";
    window_class.hIconSm       = nullptr;

    m_class_atom = RegisterClassExA(&window_class);

    if (in_window_name == nullptr)
        in_window_name = "ZenoApplication";

    const uint32_t window_style = WS_CAPTION | WS_SYSMENU;

    m_handle = CreateWindowExA(
        0, MAKEINTATOM(m_class_atom), in_window_name, window_style, in_pos_x, in_pos_y, in_size_x, in_size_y, nullptr, nullptr, m_application_instance, nullptr
    );

    int32_t error;
    if (!m_handle)
        error = GetLastError();

    ShowWindow(m_handle, SW_SHOWDEFAULT);
}

void window_t::loop(std::function<bool()> engine_loop_callback)
{
    MSG  message {};

    bool done = false;

    while (!done)
    {
        if (!queue_is_ok(&message, done))
            continue;

        done = engine_loop_callback();
    }
}

window_t::window_size_t window_t::get_window_size() const
{
    RECT rect;
    GetClientRect(m_handle, &rect);

    return
    {
        rect.right - rect.left, rect.bottom - rect.top
    };
}

bool window_t::queue_is_ok(MSG* in_message, bool& out_done)
{
    if (PeekMessageA(in_message, nullptr, 0, 0, PM_REMOVE))
    {
        if (in_message->message == WM_QUIT)
        {
            out_done = true;
            return false;
        }
        DispatchMessageA(in_message);
    }

    return true;
}

LRESULT window_t::window_procedure(HWND window_handle, UINT message_id, WPARAM w_param, LPARAM l_param)
{
    switch (message_id)
    {
        case WM_DESTROY:
        {
            PostQuitMessage(0);

            return 0;
        }
        default:
            return DefWindowProcA(window_handle, message_id, w_param, l_param);
    }
}
}  //  namespace zeno::sys