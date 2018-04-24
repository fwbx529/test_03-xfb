
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MeshReader.h"
#include "LoadShaders.h"

using namespace std;

float aspect;

void Initialize(MeshReader& mesh_reader)
{
    glm::mat4 model;
    glm::mat4 view(glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::mat4 projection(glm::perspective(glm::radians(60.0f), 1.0f / aspect, 0.1f, 100.0f));
    mesh_reader.InitXfb();
    mesh_reader.SetMatrix(model, view, projection);
    mesh_reader.ReadObjXfb("../data/bunny.obj");
}

void Display(MeshReader& mesh_reader)
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static const float white[] = { 1.0f, 1.0f, 1.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, white);
    mesh_reader.DrawXfb();
}

int main(int argc, char** argv)
{
    const int width = 800;
    const int height = 600;
    aspect = float(height) / float(width);

    glfwInit();
    glfwWindowHint(GLFW_SAMPLES, 4);
    GLFWwindow* window = glfwCreateWindow(width, height, "test-xfb", NULL, NULL);
    glfwMakeContextCurrent(window);
    glewExperimental = GL_TRUE;
    glewInit();

    MeshReader mesh_reader;
    Initialize(mesh_reader);
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