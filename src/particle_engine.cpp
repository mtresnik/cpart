

#include "particle_engine.h"
#include "glad/glad.h"

#define GRAVITY (-1)

#include <string>
#include <fstream>
#include <iterator>
#include <stdexcept>

std::string read(const std::string& filename) {
    std::ifstream file(filename, std::ios_base::binary | std::ios_base::in);
    if(!file.is_open())
        throw std::runtime_error("Failed to open " + filename);
    using Iterator = std::istreambuf_iterator<char>;
    std::string content(Iterator{file}, Iterator{});
    if(!file)
        throw std::runtime_error("Failed to read " + filename);
    return content;
}


void ParticleEngine::update(float dt) {
    for(auto &particle : particles) {
        if (particle->inAir) {
            particle->speed.y -= GRAVITY * dt;
            particle->position += particle->speed * dt;
            if (particle->position.y <= 0.0f) {
                particle->position.y = 0.0f;
                particle->speed.y = 0.0f;
                particle->inAir = false;
            }
        }
    }
}

void ParticleEngine::render() {
    // initialize correct program
    unsigned int indices[] = { 0, 1, 2 };
    glUseProgram(program);
    glBindVertexArray(vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    for(auto &particle: particles) {
        glm::vec2 pos = particle->position;
        GLfloat vertices[] = {
                -0.5f, -0.5f,
                0.5f, 0.5f,
                0.0f,0.5f
        };
        glEnableVertexAttribArray(vpos_location);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) * 4, &vertices[0], GL_STATIC_DRAW);
        glVertexAttribPointer(vpos_location, 2, GL_FLOAT, GL_FALSE, 8, nullptr);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, GL_NONE);
    }
}

void ParticleEngine::init() {
    // load vertex shader and fragment shader from files

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    std::string vertexShader = read("vertex_shader.glsl");
    std::string fragmentShader = read("fragment_shader.glsl");
    GLuint vertex_shader, fragment_shader;
    vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, reinterpret_cast<const GLchar *const *>(&vertexShader), NULL);
    glCompileShader(vertex_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, reinterpret_cast<const GLchar *const *>(&fragmentShader), NULL);
    glCompileShader(fragment_shader);

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glUseProgram(program);

    uColor_location = 0;
    vpos_location = 1;

    particles.push_back(new Particle(glm::vec2(0.0, 1.0), 4));
}
