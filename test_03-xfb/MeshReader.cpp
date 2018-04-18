
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

void MeshReader::Init(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    ShaderInfo shader_info[] =
    {
        { GL_VERTEX_SHADER, "meshreader.render.vs.glsl" },
        { GL_FRAGMENT_SHADER, "meshreader.render.fs.glsl" },
        { GL_NONE, NULL }
    };
    render_prog = LoadShaders(shader_info);
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

void MeshReader::SetMatrix(glm::mat4& model, glm::mat4& view, glm::mat4& projection)
{
    glUseProgram(render_prog);

    GLuint model_matrix_loc = glGetUniformLocation(render_prog, "model_matrix");
    GLuint view_matrix_loc = glGetUniformLocation(render_prog, "view_matrix");
    GLuint projection_matrix_loc = glGetUniformLocation(render_prog, "projection_matrix");

    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, &projection[0][0]);

    glUseProgram(0);
}

void MeshReader::Draw()
{
    glUseProgram(render_prog);
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, 3 * face_count, GL_UNSIGNED_INT, NULL);
    glBindVertexArray(0);
}

void MeshReader::Free()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(2, vbo);
}