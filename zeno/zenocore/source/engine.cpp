#include <zeno.hpp>

namespace zeno::core
{
engine_t::engine_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y)
{
    std::cout << "running\n";

    //  initiate SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        //  TODO: add error handling
        //  SDL initiation failed
    }

    //  create opengl window with sdl
    m_window = SDL_CreateWindow(
        in_window_name, in_pos_x, in_pos_y, in_size_x, in_size_y,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (m_window == NULL)
    {
        //  TODO: add error handling
        //  window creation failed
    }
}

void engine_t::run()
{
    SDL_Event event;

    //  main loop
    while (true)
    {
        //  handle events
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_QUIT:
                    break;

                case SDL_WINDOWEVENT:
                {
                }
            }
        }
    }

    SDL_DestroyWindow(m_window);
    SDL_Quit();
}


std::string engine_t::load_shaders(const char* shader_path) const
{
    std::ifstream shader_file(shader_path, std::ios::in);
    std::string   shader_string;

    if (shader_file.is_open())
    {
        std::string shader_line = "";
        while (not shader_file.eof())
        {
            std::getline(shader_file, shader_line);
            shader_string.append(shader_line + "\n");
        }
    }

    return shader_string;
}

}  //  namespace zeno::core
