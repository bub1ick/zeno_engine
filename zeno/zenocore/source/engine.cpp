#include <zeno.hpp>

namespace zeno::core
{
engine_t::engine_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y)
{
    m_window_ptr = plat::create_platform_window(in_window_name, in_pos_x, in_pos_y, in_size_x, in_size_y);

    m_renderer_ptr = new dx11::renderer_t(m_window_ptr);

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);

    SDL_Window*   window;

    window = SDL_CreateWindow(
        "test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1260, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI
    );
}

void engine_t::run()
{
    plat::platform_loop(
        [this] {

        }
    );

    SDL_Quit();
}

}  //  namespace zeno::core
