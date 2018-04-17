#pragma once

#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

class MeshReader
{
public:
    MeshReader();
    void ReadObj(const string obj_filename);
    void Draw();
    void Free();
private:
    GLuint vao;
    GLuint vbo[2];
    unsigned int face_count;
};