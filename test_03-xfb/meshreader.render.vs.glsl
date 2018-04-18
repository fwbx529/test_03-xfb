#version 450

layout(location = 0) in vec3 position;
uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;
out vec4 world_position;

void main()
{
    world_position = model_matrix * vec4(position, 1.0f);
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0f);
}
