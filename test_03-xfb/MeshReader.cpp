
#include "MeshReader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

MeshReader::MeshReader()
{
    glCreateVertexArrays(1, &vao);
    glCreateBuffers(3, array_buffer);
    glCreateBuffers(1, &element_buffer);
}

void MeshReader::Init()
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
    vector<glm::vec3> vertex;
    vector<glm::ivec3> face;
    vector<glm::vec3> normal;

    ifstream fin(obj_filename);
    string cmd;
    bool vs_normal = false;
    while (fin >> cmd)
    {
        GLfloat xf, yf, zf;
        GLuint xi, yi, zi;

        if (cmd == "v")
        {
            fin >> xf >> yf >> zf;
            vertex.push_back(glm::vec3(xf, yf, zf));
        }
        else if (cmd == "f")
        {
            fin >> xi >> yi >> zi;
            xi--; yi--; zi--;
            face.push_back(glm::ivec3(xi, yi, zi));
        }
        else if (cmd == "vn")
        {
            vs_normal = true;
            //to be updated
        }
    }
    fin.close();
    vertex_count = (int)vertex.size();
    face_count = (int)face.size();

    if (!vs_normal)
    {
        normal = vector<glm::vec3>(vertex_count, glm::vec3(0, 0, 0));
        for (unsigned int f = 0; f < face_count; f++)
        {
            glm::ivec3 f_id(face[f]);
            glm::vec3 vx(vertex[f_id.x]);
            glm::vec3 vy(vertex[f_id.y]);
            glm::vec3 vz(vertex[f_id.z]);
            glm::vec3 n = glm::cross(vy - vx, vz - vx);
            normal[f_id.x] += n;
            normal[f_id.y] += n;
            normal[f_id.z] += n;
        }
        for (unsigned int v = 0; v < vertex_count; v++)
        {
            normal[v] = glm::normalize(normal[v]);
        }
    }

    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, array_buffer[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(0);
    glNamedBufferStorage(array_buffer[0], vertex_count * sizeof(glm::vec3), vertex.data(), 0);

    glBindBuffer(GL_ARRAY_BUFFER, array_buffer[1]);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(1);
    glNamedBufferStorage(array_buffer[1], vertex_count * sizeof(glm::vec3), normal.data(), 0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glNamedBufferStorage(element_buffer, face_count * sizeof(glm::ivec3), face.data(), 0);

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
    glUseProgram(0);
    glBindVertexArray(0);
}

void MeshReader::Free()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(3, array_buffer);
    glDeleteBuffers(1, &element_buffer);
}