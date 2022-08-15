#pragma once

namespace zeno::core
{

class ZENO_API engine_t
{
public:
    engine_t(
        const char* in_window_name, int32_t in_pos_x = CW_USEDEFAULT, int32_t in_pos_y = CW_USEDEFAULT,
        int32_t in_size_x = CW_USEDEFAULT, int32_t in_size_y = CW_USEDEFAULT
    );

    void run();

private:
    plat::window_t*   m_window_ptr;

    dx11::renderer_t* m_renderer_ptr;

    std::string       m_command_line = "";
};
}  //  namespace zeno::core