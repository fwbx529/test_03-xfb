
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MeshReader.h"
#include "LoadShaders.h"

using namespace std;

float aspect;

void Display(MeshReader& mesh_reader)
{
    ShaderInfo shader_info[] =
    {
        { GL_VERTEX_SHADER, "render.vs.glsl" },
        { GL_FRAGMENT_SHADER, "render.fs.glsl" },
        { GL_NONE, NULL }
    };
    GLuint render_prog = LoadShaders(shader_info);
    glUseProgram(render_prog);

    GLuint model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix");
    GLuint view_matrix_loc = glGetUniformLocation(render_prog, "view_matrix");
    GLuint projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix");

    glm::mat4 model;
    glm::mat4 view(glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::mat4 projection(glm::perspective(glm::radians(60.0f), 1.0f / aspect, 0.1f, 100.0f));
    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, &projection[0][0]);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    mesh_reader.Draw();
}

int main(int argc, char** argv)
{
    const int width = 800;
    const int height = 600;
    aspect = float(height) / float(width);

    glfwInit();
    GLFWwindow* window = glfwCreateWindow(width, height, "test-xfb", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    MeshReader mesh_reader;
    mesh_reader.ReadObj("../data/bunny.obj");
    while (!glfwWindowShouldClose(window))
    {
        Display(mesh_reader);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    mesh_reader.Free();

    glfwDestroyWindow(window);
    glfwTerminate();
}