#version 460 core

layout (location = 0) in vec3 vertex_positions;

void main()
{
    gl_Position = vec4(vertex_positions.x, vertex_positions.y, vertex_positions.z, 1.0);
}