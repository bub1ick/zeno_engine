#include <zeno.hpp>

namespace zeno::plat::win32
{
win32_window_t::win32_window_t(
    const char* in_title, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y,
    HINSTANCE in_instance
)
{
    m_title              = in_title;
    m_window_dimenstions = {in_pos_x, in_pos_y, in_size_x, in_size_y};
    m_instance           = in_instance;

    m_class.cbSize        = sizeof(WNDCLASSEXA);
    m_class.style         = CS_VREDRAW | CS_HREDRAW | CS_OWNDC;
    m_class.lpfnWndProc   = m_window_procedure;
    m_class.cbClsExtra    = 0;
    m_class.cbWndExtra    = sizeof(this);
    m_class.hInstance     = m_instance;
    m_class.hIcon         = nullptr;
    m_class.hCursor       = LoadCursor(m_instance, IDC_ARROW);
    m_class.hbrBackground = nullptr;
    m_class.lpszMenuName  = nullptr;
    m_class.lpszClassName = "zeno application";
    m_class.hIconSm       = nullptr;

    m_class_atom = RegisterClassExA(&m_class);
    if (not m_class_atom)
        throw std::runtime_error("Could not register a window class.");

    m_handle = CreateWindowExA(
        WS_EX_OVERLAPPEDWINDOW, MAKEINTATOM(m_class_atom), m_title, WS_OVERLAPPEDWINDOW, m_window_dimenstions.pos_x,
        m_window_dimenstions.pos_y, m_window_dimenstions.size_x, m_window_dimenstions.size_y, nullptr, nullptr,
        m_class.hInstance, this
    );
    if (not m_handle)
        throw std::runtime_error("Could not create a window (get a window handle).");

    ShowWindow(m_handle, SW_SHOWDEFAULT);
}

LRESULT win32_window_t::m_window_procedure(HWND window, uint32_t message_id, WPARAM w_param, LPARAM l_param)
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
            win32_window_t* this_window = reinterpret_cast<win32_window_t*>(GetWindowLongPtrA(window, GWLP_USERDATA));

            return 0;
        }
        case WM_DESTROY:
            return 0;

        default:
            return DefWindowProcA(window, message_id, w_param, l_param);
    }
}

}  //  namespace zeno::plat::win32
