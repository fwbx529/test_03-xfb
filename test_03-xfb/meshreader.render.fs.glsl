#version 450

out vec4 fcolor;
in vec4 world_position;

void main()
{
    float colorz = clamp(world_position.z + 0.3f, 0.0f, 1.0f);
    fcolor = vec4(0.0f, colorz, 0.0f, 1.0f);
}