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

vec3 collision(vec3 pos, vec3 pos_next)
{
    for (int f = 0; f < face_count; f++)
    {
        vec3 vx = texelFetch(world_position_tbo, 3 * f).xyz;
        vec3 vy = texelFetch(world_position_tbo, 3 * f + 1).xyz;
        vec3 vz = texelFetch(world_position_tbo, 3 * f + 2).xyz;
        vec3 normal = cross(vy - vx, vz - vx);
        normal = normalize(normal);
        vec3 direction = pos_next - pos;

        float a = dot(pos - vx, normal), b = dot(pos_next - vx, normal);
        if (a >= 0 && b < 0)
        {
            float portion = a / (a - b);
            vec3 collide = pos + direction * portion;
            vec3
                cross12 = cross(vx - collide, vy - collide),
                cross23 = cross(vy - collide, vz - collide),
                cross31 = cross(vz - collide, vx - collide);
            if (dot(cross12, normal) >= 0 && dot(cross23, normal) >= 0 && dot(cross31, normal) >= 0) return normal;
        }
    }
    return vec3(0, 0, 0);
}

void main()
{
    world_position = model_matrix * vec4(position, 1.0f);
    world_position_xfb = world_position.xyz + velocity * time_second;
    world_velocity_xfb = velocity + vec3(0, -time_second * 0.5f, 0);

    vec3 normal = collision(world_position.xyz, world_position_xfb);
    world_velocity_xfb = world_velocity_xfb - normal * (2 * dot(normal, world_velocity_xfb));
    if (world_position_xfb.y < -1.0f)
    {
        world_position_xfb.x = 0.0f;
        world_position_xfb.y += 1.8f;
        world_position_xfb.z = 0.0f;
        world_velocity_xfb *= vec3(0.5f, 0.1f, 0.5f);
    }
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
}
