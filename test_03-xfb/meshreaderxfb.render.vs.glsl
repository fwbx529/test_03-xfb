#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
out vec4 world_position;
out vec4 world_normal;
out vec3 world_position_xfb;

void main()
{
    world_position = model_matrix * vec4(position, 1.0f);
    world_position_xfb = world_position.xyz + vec3(0.1f, 0.1f, 0.0f);
    world_normal = model_matrix*vec4(normal, 0.0f);
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
}
