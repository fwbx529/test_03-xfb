#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 velocity;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform float time_second;
out vec4 world_position;
out vec3 world_position_xfb;
out vec3 world_velocity_xfb;

void main()
{
    world_position = model_matrix * vec4(position, 1.0f);
    world_position_xfb = world_position.xyz + vec3(0.0f, -time_second / 10, 0.0f);
    world_velocity_xfb = velocity;
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
}
