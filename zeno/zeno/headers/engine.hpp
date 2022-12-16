#pragma once

namespace zeno
{

class ZENO_API engine_t
{
public:
    engine_t(
        const char* in_window_name,
        int32_t     in_pos_x  = CW_USEDEFAULT,
        int32_t     in_pos_y  = CW_USEDEFAULT,
        int32_t     in_size_x = CW_USEDEFAULT,
        int32_t     in_size_y = CW_USEDEFAULT
    );

    int32_t run();

private:
    void            loop();

    sys::window_t   m_window;

    gfx::renderer_t m_renderer;

    std::string     m_command_line = "";

    bool            m_game_is_done = false;
};

}  //  namespace zeno