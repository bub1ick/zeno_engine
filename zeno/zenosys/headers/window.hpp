#pragma once

namespace zeno::sys
{
///  @brief class that represents a window in the Windows OS
class ZENO_API window_t
{
public:
    ///  @brief a window constructor which creates the window in the system
    ///  @param in_window_name a title of the window displayed in the taskbar, task manager, etc.
    ///  @param in_pos_x position of the window on the screen X coordinate
    ///  @param in_pos_y position of the window on the screen Y coordinate
    ///  @param in_size_x window width
    ///  @param in_size_y window height
    window_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y);

    ///  @brief main system event loop
    ///  @param engine_loop callback to the engine loop function, which handles engine systems
    void loop(std::function<bool()> engine_loop);

    ///  @brief get the win32 window handle
    ///  @return the handle
    HWND get_handle() const { return m_handle; };

    ///  @brief defines the size of the client area of the window;
    struct dimentions_t
    {
        int32_t width;
        int32_t height;
    };

    dimentions_t get_dimentions() const;

private:
    ///  @brief win32 window procedure to handle win32 events
    ///  @param window_handle handle to a window
    ///  @param message_id message dispatched from the queue
    ///  @param w_param
    ///  @param l_param
    ///  @return message-defined value
    static LRESULT window_procedure(HWND window_handle, UINT message_id, WPARAM w_param, LPARAM l_param);

    ///  @brief preprocesses the message and performs checks on it, as well as handles it
    ///  @param in_message the message to preprocess
    ///  @param out_done if we should quit the application or not
    ///  @return whether we can safely call the engine loop
    bool           queue_is_ok(MSG* in_message, bool& out_done);

    ///  @brief win32 application instance
    HINSTANCE      m_application_instance;

    ///  @brief win32 atom representing class instead of class name string
    ATOM           m_class_atom;

    ///  @brief win32 window handle
    HWND           m_handle;
};
}  //  namespace zeno::sys