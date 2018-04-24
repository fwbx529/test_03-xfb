#version 450

out vec4 fcolor;
in vec4 world_position;
in vec4 world_normal;

void main()
{
    float color = clamp(world_normal.z, 0.0f, 1.0f);
    fcolor = vec4(color, color, color, 1.0f);
}