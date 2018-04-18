#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "LoadShaders.h"

using namespace std;

class MeshReader
{
public:
    MeshReader();
    void Init(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void ReadObj(const string obj_filename);
    void SetMatrix(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void Draw();
    void Free();
private:
    GLuint vao;
    GLuint vbo[2];
    GLuint render_prog;
    unsigned int face_count;
};