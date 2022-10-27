#include <zeno.hpp>

namespace zeno::sys
{
window_t::window_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y)
{
    m_application_instance = GetModuleHandleA(0);


    //  set up the window class
    WNDCLASSEXA window_class {};
    window_class.cbSize        = sizeof(WNDCLASSEXA);     //  size of the structure
    window_class.style         = 0;                       //  no special styles required
    window_class.lpfnWndProc   = window_procedure;        //  use our static window procedure
    window_class.cbClsExtra    = 0;                       //  no extra memory for now
    window_class.cbWndExtra    = 0;                       //  no extra memory for now
    window_class.hInstance     = m_application_instance;  //  the hInstance is the .exe (because zenosys is static lib)
    window_class.hIcon         = nullptr;                 //  no icon loaders for now
    window_class.hCursor       = nullptr;                 //  no cursor loaders for now
    window_class.hbrBackground = nullptr;                 //  no background color as we don't use gdi for rendering
    window_class.lpszMenuName  = nullptr;                 //  don't use menu (games don't have toolbars lmao)
    window_class.lpszClassName = "ZenoApplication";       //  class name to identify it in the system
    window_class.hIconSm       = nullptr;                 //  no icon loaders for now

    //  register our class
    m_class_atom = RegisterClassExA(&window_class);

    //  check if window title is not empty
    if (in_window_name == nullptr)
        in_window_name = "ZenoApplication";  //  otherwise provide with default name for the .exe

    //  create our window, but don't show it yet untill we are fully initialized
    m_handle = CreateWindowExA(
        0,
        MAKEINTATOM(m_class_atom),
        in_window_name,
        0,
        in_pos_x,
        in_pos_y,
        in_size_x,
        in_size_y,
        nullptr,
        nullptr,
        m_application_instance,
        nullptr
    );

    int32_t error;
    if (not m_handle)
        error = GetLastError();

    ShowWindow(m_handle, SW_SHOWDEFAULT);
}

void window_t::loop(std::function<bool()> engine_loop_callback)
{
    //  create a message
    MSG  message {};

    bool done = false;

    //  as long as we are not done
    while (not done)
    {
        //  check whether we are safe to call engine loop and if we can handle message
        if (not queue_is_ok(&message, done))  
            continue;

        //  do the engine suff
        done = engine_loop_callback();
    }
}

bool window_t::queue_is_ok(MSG* in_out_message, bool& out_done)
{
    //  check the message queue if it's empty
    int32_t queue_is_empty =
        not PeekMessageA(in_out_message, m_handle, 0, 0, PM_NOREMOVE);  //  returns 0 if empty so we negate it

    if (queue_is_empty)
        return true;  //  go straight to engine loop if no messages were found

    //  check the queue for errors
    switch (GetMessageA(in_out_message, m_handle, 0, 0))
    {
        case 0:  //  WM_QUIT -> must quit the application
        {
            out_done = true;  //  mark as done

            return false; // don't call engine loop
        }
        case -1:  //  there was an error
        {
            int32_t error_code = GetLastError();  //  get error

            //  TODO: handle error

            PostQuitMessage(0);  //  leave the application
            return false;        //  jump to next loop iter to handle WM_QUIT
        }
        default:  //  if everything's fine
        {
            DispatchMessageA(in_out_message);  //  dispatch the message

            return true;
        }
    }
}

LRESULT window_t::window_procedure(HWND window_handle, UINT message_id, WPARAM w_param, LPARAM l_param)
{
    switch (message_id)
    {
        case WM_DESTROY:
            return 0;

        default:
            return DefWindowProcA(window_handle, message_id, w_param, l_param);
    }
}
}  //  namespace zeno::sys