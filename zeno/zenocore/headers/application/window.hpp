#pragma once

namespace zeno
{
    class window_t
    {
    public:
        window_t(const char* window_name, int32_t pos_x, int32_t pos_y, int32_t size_x, int32_t size_y, HINSTANCE instance, int32_t show_command);

        HWND get_window_handle() const;

        ATOM get_class_atom() const;

    private:

        WNDCLASSEXA _class;

        ATOM _class_atom;

        HWND _handle;

        int32_t _pos_x;

        int32_t _pos_y;

        int32_t _size_x;

        int32_t _size_y;

        const char* _title;

        void set_up_window_class(HINSTANCE instance);

        void create_window(int32_t show_command);

        static LRESULT _window_procedure(HWND window, uint32_t message_id, WPARAM w_param, LPARAM l_param);
    };
}