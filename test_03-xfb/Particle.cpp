
#include "Particle.h"

#include <iostream>
#include <vector>

struct particle_status
{
    glm::vec3 poisition;
    glm::vec3 velocity;
};

float random_1()
{
    return (float)rand() / RAND_MAX;
}

Particle::Particle()
{
    glCreateVertexArrays(2, vao);
    glCreateBuffers(2, vbo);
    frame_even = true;
}

void Particle::Init()
{
    ShaderInfo shader_info[] =
    {
        { GL_VERTEX_SHADER, "particle.render.vs.glsl" },
        { GL_FRAGMENT_SHADER, "particle.render.fs.glsl" },
        { GL_NONE, NULL }
    };
    particle_prog = LoadShaders(shader_info);

    static const char * varyings[] =
    {
        "world_position_xfb", "world_velocity_xfb"
    };
    glTransformFeedbackVaryings(particle_prog, 2, varyings, GL_INTERLEAVED_ATTRIBS);
    glLinkProgram(particle_prog);
}

void Particle::SetInitial(const int count)
{
    last_frame = clock();
    particle_count = count;
    srand((unsigned)time(NULL));
    vector<particle_status> status(count);

    for (int i = 0; i < count; i++)
    {
        status[i].poisition = glm::vec3(0.0f, 0.8f, 0.0f);
        status[i].velocity = glm::vec3(
            (2 * random_1() - 1) / 5,
            (5 * random_1() - 5),
            (2 * random_1() / 5));
    }

    for (int i = 0; i < 2; i++)
    {
        glBindVertexArray(vao[i]);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[i]);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(particle_status), NULL);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(particle_status), (void*)sizeof(glm::vec3));
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
    }
    glNamedBufferStorage(vbo[0], 2 * count * sizeof(glm::vec3), status.data(), 0);
    glNamedBufferStorage(vbo[1], 2 * count * sizeof(glm::vec3), NULL, 0);
}

void Particle::SetMatrix(const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    glUseProgram(particle_prog);
    SetMatrixProg(particle_prog, model, view, projection);
    glUseProgram(0);
}

void Particle::Draw(const int face_count)
{
    float time_second = (float)(clock() - last_frame) / 1000;
    last_frame = clock();
    glUseProgram(particle_prog);
    GLuint time_second_loc = glGetUniformLocation(particle_prog, "time_second");
    GLuint face_count_loc = glGetUniformLocation(particle_prog, "face_count");
    glUniform1f(time_second_loc, time_second);
    glUniform1i(face_count_loc, face_count);

    if (frame_even)
    {
        glBindVertexArray(vao[0]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[1]);
    }
    else
    {
        glBindVertexArray(vao[1]);
        glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, vbo[0]);
    }

    glBeginTransformFeedback(GL_POINTS);
    glPointSize(2);
    glDrawArrays(GL_POINTS, 0, particle_count);
    glEndTransformFeedback();

    frame_even = !frame_even;
    glUseProgram(0);
    glBindVertexArray(0);
}

void Particle::Free()
{
    glDeleteVertexArrays(2, vao);
    glDeleteBuffers(2, vbo);
}

void Particle::SetMatrixProg(GLuint& prog, const glm::mat4& model, const glm::mat4& view, const glm::mat4& projection)
{
    GLuint model_matrix_loc = glGetUniformLocation(prog, "model_matrix");
    GLuint view_matrix_loc = glGetUniformLocation(prog, "view_matrix");
    GLuint projection_matrix_loc = glGetUniformLocation(prog, "projection_matrix");

    glUniformMatrix4fv(model_matrix_loc, 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(view_matrix_loc, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(projection_matrix_loc, 1, GL_FALSE, &projection[0][0]);
}