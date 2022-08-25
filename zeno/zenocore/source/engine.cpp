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

    //  set required opengl standard
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);

    //  create opengl window with sdl
    m_window = SDL_CreateWindow(
        in_window_name, in_pos_x, in_pos_y, in_size_x, in_size_y,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI
    );

    if (m_window == NULL)
    {
        //  TODO: add error handling
        //  window creation failed
    }

    //  get opengl context
    m_opengl_context = SDL_GL_CreateContext(m_window);

    if (glewInit() != GLEW_OK)
    {
        //  TODO: add error handling
        //  glew init failed
    }
}

void engine_t::run()
{
    const char* vertex_shader =
        "#version 460 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
        "}\0";

    const char* fragment_shader =
        "#version 460 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
        "}\0";

    float triangle_vertices [3][2] = {
        {-1.0f, -1.0f},
        { 1.0f, -1.0f},
        { 0.0f,  1.0f}
    };

    //  create a shader program and save its id
    uint32_t shader_program_id = create_shader_program(vertex_shader, fragment_shader);
    //  std::cout << shader_program_id; // test

    //  create vertex buffer object and vertex array object
    uint32_t VBO;
    uint32_t VAO;

    //  generate buffer and array objects
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    //  bind vertex array to vertex object  (?)
    glBindVertexArray(VAO);

    //  bind array buffer to vbo
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    //  buffer vertece data
    glBufferData(GL_ARRAY_BUFFER, sizeof(triangle_vertices), triangle_vertices, GL_STATIC_DRAW);

    //  define vertex attribute data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(triangle_vertices [0]), (void*) 0);
    glEnableVertexAttribArray(0);

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
                    switch (event.window.event)
                    {
                        case SDL_WINDOWEVENT_RESIZED:
                            glViewport(0, 0, event.window.data1, event.window.data2);
                    }
                }
            }
        }

        //  set clear color and clear the window
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        //  draw our vertices using our shader program
        glUseProgram(shader_program_id);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        //  switch current frame
        SDL_GL_SwapWindow(m_window);
    }

    //  clean up
    SDL_GL_DeleteContext(m_opengl_context);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

uint32_t engine_t::compile_shader(uint32_t shader_type, const char* shader)
{
    //  create shader (GL_VERTEX_SHADER and GL_FRAGMENT_SHADER)
    uint32_t shader_id = glCreateShader(shader_type);

    //  change shader source to the given one
    glShaderSource(shader_id, 1, &shader, NULL);

    //  compile the shader
    glCompileShader(shader_id);
    int32_t param;
    glGetShaderiv(shader_id, GL_COMPILE_STATUS, &param);
    if (param != GL_TRUE)
    {
        //  TODO: add error handling
        //  compilation failed
        return -1;
    }

    return shader_id;
}

uint32_t engine_t::create_shader_program(const char* vertex_shader, const char* fragment_shader)
{
    //  create shader program
    uint32_t shader_program = glCreateProgram();

    //  compile vertex and fragment shaders
    uint32_t vertex_shader_id   = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    uint32_t fragment_shader_id = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);

    //  attach shaders to shader program
    glAttachShader(shader_program, vertex_shader_id);
    glAttachShader(shader_program, fragment_shader_id);

    //  link the program object
    glLinkProgram(shader_program);
    int32_t param;
    glGetProgramiv(shader_program, GL_LINK_STATUS, &param);
    if (param != GL_TRUE)
    {
        //  TODO: add error handling
        //  link failed
    }

    return shader_program;
}

}  //  namespace zeno::core
