#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "LoadShaders.h"

using namespace std;

class Particle
{
public:
    Particle();
    void Init();
    void SetInitial(const int count);
    void SetMatrix(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
    void Draw();
private:
    GLuint vao[2];
    GLuint vbo[2];
    GLuint particle_prog;
    int particle_count;
    bool frame_even;

    void SetMatrixProg(GLuint& prog, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
};