#pragma once

namespace zeno::core
{

class ZENO_API engine_t
{
public:
    engine_t(
        const char* in_window_name, int32_t in_pos_x = 0, int32_t in_pos_y = 0, int32_t in_size_x = 0,
        int32_t in_size_y = 0
    );

    void run();

private:
    std::string   m_command_line = "";

    SDL_Window*   m_window;

    SDL_GLContext m_opengl_context;

    bool          m_running = true;

    uint32_t      compile_shader(uint32_t shader_type, const char* shader);

    uint32_t      create_shader_program(const char* vertex_shader, const char* fragment_shader);
};
}  //  namespace zeno::core