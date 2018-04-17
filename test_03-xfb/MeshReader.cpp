
#include "MeshReader.h"

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

MeshReader::MeshReader()
{
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(2, vbo);
}

void MeshReader::ReadObj(const string obj_filename)
{
    vector<GLfloat> vertex;
    vector<GLuint> face;

    ifstream fin(obj_filename);
    char vf;
    while (fin >> vf)
    {
        GLfloat vx, vy, vz;
        GLuint fx, fy, fz;
        switch(vf)
        {
        case 'v':
            fin >> vx >> vy >> vz;
            vertex.push_back(vx);
            vertex.push_back(vy);
            vertex.push_back(vz);
            break;
        case 'f': 
            fin >> fx >> fy >> fz;
            fx--; fy--; fz--;
            face.push_back(fx);
            face.push_back(fy);
            face.push_back(fz);
            break;
        }
    }
    fin.close();
    face_count = (int)face.size();

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glNamedBufferStorage(vbo[0], vertex.size() * sizeof(GLfloat), vertex.data(), 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo[1]);
    glNamedBufferStorage(vbo[1], face.size() * sizeof(GLuint), face.data(), 0);
    glBindVertexArray(0);
}

void MeshReader::Draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3 * face_count, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void MeshReader::Free()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(2, vbo);
}