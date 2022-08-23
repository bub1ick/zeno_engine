#include <zeno.hpp>

namespace zeno::core
{
engine_t::engine_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        //  couldn't init
    }

    m_window = SDL_CreateWindow(
        in_window_name, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, in_size_x, in_size_y,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (m_window == NULL)
    {
        //  window creation failed
    }

    //  create opengl context
    m_opengl_context = SDL_GL_CreateContext(m_window);

    //  initialize glew
    if (glewInit() != GLEW_OK)
    {
        //  glew init failed
    }
}

void engine_t::run()
{
    const float triangle_vertecies [3][2] = {
        {-1, -1},
        { 0,  1},
        { 1, -1}
    };
    SDL_Event event;

    uint32_t  triangle_id;

    glGenBuffers(1, &triangle_id);
    glBindBuffer(GL_ARRAY_BUFFER, triangle_id);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertecies), &triangle_vertecies, GL_STATIC_DRAW);

    const uint32_t attribute_index = 0;
    glVertexAttribPointer(attribute_index, 2, GL_FLOAT, GL_FALSE, sizeof(triangle_vertecies [0]), 0);
    glEnableVertexAttribArray(attribute_index);

    while (m_running)
    {
        //  process events
        while (SDL_PollEvent(&event))
        {
        }
        
        //  update event states, draw a frame etc.
        //  specify clear values
        glClearColor(0, 0, 0, 0);

        //  clear buffer
        glClear(GL_COLOR_BUFFER_BIT);

        //  render data from array
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //  update the window using opengl
        SDL_GL_SwapWindow(m_window);
    }

    //  clean up on finish
    SDL_Quit();
}

}  //  namespace zeno::core
