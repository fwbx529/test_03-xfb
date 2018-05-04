#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
out vec4 world_position;
out vec4 world_normal;

void main()
{
    world_position = model_matrix * vec4(position, 1.0f);
    world_normal = view_matrix * model_matrix * vec4(normal, 0.0f);
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
}
