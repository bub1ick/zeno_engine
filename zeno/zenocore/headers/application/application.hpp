#pragma once

#include "window.hpp"

namespace zeno
{
class application_t
{
public:
    application_t(
        const char* window_name, int32_t pos_x = CW_USEDEFAULT, int32_t pos_y = CW_USEDEFAULT,
        int32_t size_x = CW_USEDEFAULT, int32_t size_y = CW_USEDEFAULT
    );

    application_t(
        const char* window_name, int32_t argument_count, char** arguments, int32_t pos_x = CW_USEDEFAULT,
        int32_t pos_y = CW_USEDEFAULT, int32_t size_x = CW_USEDEFAULT, int32_t size_y = CW_USEDEFAULT
    );

    ~application_t();

    void run();

private:
    window_t*   _window;

    HINSTANCE   _instance;

    std::string _command_line;
};
}  //  namespace zeno