#pragma once

#include <string>
#include <utility>

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
    void SetMatrix(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
    void Draw();
    void Free();

    void InitXfb();
    void ReadObjXfb(const string obj_filename);
    void SetMatrixXfb(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
    void DrawXfb();
    void TestXfb();

    int GetFaceCount();
private:
    GLuint vao;
    GLuint array_buffer[3];
    GLuint element_buffer;
    GLuint render_prog;
    unsigned int vertex_count;
    unsigned int face_count;

    GLuint xfb_world_position;
    GLuint xfb_world_normal;
    GLuint tbo_world_position;
    GLuint renderxfb_prog;

    void SetMatrixProg(GLuint& prog, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection);
};