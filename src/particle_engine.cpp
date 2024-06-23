

#include "particle_engine.h"
#include "glad/glad.h"
#include "util.h"
#include "converter.h"

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

    glUseProgram(program);
    glBindVertexArray(vao);
    float dx = 0.1f;
    float dy = 0.1f;
    for (auto &particle : particles) {
        std::vector<float> vertexData {
                particle->position.x - dx/2,  particle->position.y + dy/2, 0.0f,
                particle->position.x + dx/2, particle->position.y + dy/2, 0.0f,
                particle->position.x + dx/2, particle->position.y - dy/2, 0.0f,
                particle->position.x - dx/2,  particle->position.y - dy/2, 0.0f
        };
        std::vector<float> colorData {
                1.0f, 0.0f, 0.0f,
                1.0f, 1.0f, 0.0f,
                1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 0.0f
        };
        glBindBuffer(GL_ARRAY_BUFFER, vbo0);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertexData[0]) * vertexData.size(),vertexData.data(),GL_STATIC_DRAW);
        GLint posLocation = glGetAttribLocation(program, "position");
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE,0,nullptr);

        glBindBuffer(GL_ARRAY_BUFFER, vbo1);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorData[0]) * colorData.size(), colorData.data(), GL_STATIC_DRAW);
        GLint colorLocation = glGetAttribLocation(program, "color");
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glDrawArrays(GL_QUADS, 0, 4);
    }

}

void ParticleEngine::init() {
    program = createShaderProgram("vertex.glsl", "fragment.glsl");

    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    vbo0 = 0;
    vbo1 = 0;
    glGenBuffers(1, &vbo0);
    glGenBuffers(1, &vbo1);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void ParticleEngine::destroy() {
    glDeleteProgram(program);
}

void ParticleEngine::onClick(double xPos, double yPos) {
    printf("onClick\txPos: %f\tyPos:%f\n", xPos, yPos);
    double rx, ry;
    Converter::getInstance()->convert(xPos, yPos, &rx, &ry);
    particles.push_back(new Particle(glm::vec2(rx, ry), 4));
}


ParticleEngine* ParticleEngine::instancePtr = nullptr;
