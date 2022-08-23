#include <zeno.hpp>

namespace zeno::core
{

uint32_t engine_t::setup_shader(uint32_t shader_type, std::string& shader)
{
    //  create a shader object of a given type and get its id
    uint32_t    shader_id = glCreateShader(shader_type);

    const char* shader_data = shader.c_str();

    //  replace the source code of a shader object with our worn
    glShaderSource(shader_id, 1, &shader_data, NULL);

    //  compile a shader string we just placed in glShaderSource
    glCompileShader(shader_id);

    //  check if the shader compiled corretly
    int32_t params;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &params);
    if (params == GL_FALSE)
    {
        //  shader compiation failed
    }
    return shader_id;
}

uint32_t engine_t::create_shaders(std::string& vertex_shader, std::string& fragment_shader)
{
    //  create a program object to attach a shader object to
    uint32_t program = glCreateProgram();

    uint32_t vertex_shader_id   = setup_shader(GL_VERTEX_SHADER, vertex_shader);
    uint32_t fragment_shader_id = setup_shader(GL_FRAGMENT_SHADER, fragment_shader);

    //  attach shaders to a program we created earlier
    glAttachShader(program, vertex_shader_id);
    glAttachShader(program, fragment_shader_id);

    glLinkProgram(program);      //  ?
    glValidateProgram(program);  //  chto naxuy

    //  delete our shader
    glDeleteShader(vertex_shader_id);
    glDeleteShader(fragment_shader_id);

    return program;
}

engine_t::engine_t(const char* in_window_name, int32_t in_pos_x, int32_t in_pos_y, int32_t in_size_x, int32_t in_size_y)
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER) != 0)
    {
        //  couldn't init
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 6);

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

    //  if true - forces glew to use modern ways to check whether a function is available
    //  use if glew version 1.13 and earlier
    glewExperimental = GL_FALSE;

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

    //  create a buffer for our memory
    glGenBuffers(1, &triangle_id);

    //  make buffer an active object
    glBindBuffer(GL_ARRAY_BUFFER, triangle_id);

    //  copy vertex data to the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertecies), &triangle_vertecies, GL_STATIC_DRAW);

    const uint32_t attribute_index = 0;
    glEnableVertexAttribArray(attribute_index);
    glVertexAttribPointer(attribute_index, 2, GL_FLOAT, GL_FALSE, sizeof(triangle_vertecies [0]), 0);

    //  create a shader
    std::string vertex_shader =
        "#version 460 core\n"  //  vrode pravilno
        "\n"
        "layout(location = 0) in vec4 position;\n"  //  0 = attribute_index
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";

    std::string frament_shader =
        "#version 460 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.0, 1.0, 1.0, 1.0);\n"
        "}\n";

    uint32_t program = engine_t::create_shaders(vertex_shader, frament_shader);

    glUseProgram(program);

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
