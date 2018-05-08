
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "MeshReader.h"
#include "Particle.h"
#include "LoadShaders.h"

using namespace std;

float aspect;
float last_frame;

void Initialize(MeshReader& mesh_reader, Particle& particle)
{
    mesh_reader.InitXfb();
    mesh_reader.ReadObjXfb("../data/bunny.obj");
    particle.Init();
    particle.SetInitial(200);
}

void Display(MeshReader& mesh_reader, Particle& particle)
{
    static float first_frame = (float)clock();
    float time_second = (float)(clock() - last_frame) / 1000;
    last_frame = (float)clock();
    float time_total = (float)(last_frame - first_frame) / 1000;
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    static const float black[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glClearBufferfv(GL_COLOR, 0, black);

    glm::mat4 model = glm::rotate(glm::mat4(), time_total, glm::vec3(0, 1, 0));
    glm::mat4 view(glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f),
                               glm::vec3(0.0f, 0.0f, 0.0f),
                               glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::mat4 projection(glm::perspective(glm::radians(90.0f), 1.0f / aspect, 0.1f, 100.0f));
    mesh_reader.SetMatrixXfb(model, view, projection);
    particle.SetMatrix(glm::mat4(), view, projection);

    mesh_reader.DrawXfb();
    particle.Draw(mesh_reader.GetFaceCount(), time_second);
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
    Particle particle;
    Initialize(mesh_reader, particle);
    while (!glfwWindowShouldClose(window))
    {
        Display(mesh_reader, particle);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    mesh_reader.Free();
    particle.Free();

    glfwDestroyWindow(window);
    glfwTerminate();
}