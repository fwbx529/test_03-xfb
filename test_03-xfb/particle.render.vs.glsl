#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 velocity;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
uniform float time_second;
uniform int face_count;
layout(binding = 0) uniform samplerBuffer world_position_tbo;
out vec4 world_position;
out vec3 world_position_xfb;
out vec3 world_velocity_xfb;

bool collision(vec3 pos)
{
    for (int f = 0; f < face_count; f++)
    {
        vec3 vertex = texelFetch(world_position_tbo, f).xyz;
        if (distance(pos, vertex) < 0.1) return true;
    }
    return false;
}

void main()
{
    world_position = model_matrix * vec4(position, 1.0f);
    world_position_xfb = world_position.xyz + velocity * time_second;
    world_velocity_xfb = velocity + vec3(0, -time_second * 0.5f, 0);
    if (collision(world_position.xyz) == true) world_velocity_xfb = world_velocity_xfb + vec3(0, 1, 0);
    if (world_position_xfb.y < -1.0f)
    {
        world_position_xfb.x = 0.0f;
        world_position_xfb.y += 1.8f;
        world_position_xfb.z = 0.0f;
        world_velocity_xfb *= vec3(1.0f, 0.1f, 1.0f);
    }
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
}
