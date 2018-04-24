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
    void Init();
    void ReadObj(const string obj_filename);
    void SetMatrix(glm::mat4& model, glm::mat4& view, glm::mat4& projection);
    void Draw();
    void Free();

    void InitXfb();
    void ReadObjXfb(const string obj_filename);
    void DrawXfb();
private:
    GLuint vao;
    GLuint array_buffer[3];
    GLuint element_buffer;
    GLuint render_prog;
    unsigned int vertex_count;
    unsigned int face_count;

    GLuint renderxfb_world_position;
    GLuint renderxfb_prog;
};