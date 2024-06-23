

#include "particle_engine.h"
#include "glad/glad.h"
#include "util.h"
#include "converter.h"

#define GRAVITY (0.1f)
#define PARTICLE_SPEED (0.2f)
#define PARTICLE_WIDTH (8)

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

void ParticleEngine::updateSand(Particle* particle, float dt) {
    if (particle->inAir) {
        int row, col;
        getRowCol(particle->position.x, particle->position.y, &row, &col);
        if (row > 0 && col > 0 && row < rows && col < cols) {
            Particle* below = nullptr;
            Particle* bottomLeft = nullptr;
            Particle* bottomRight = nullptr;
            if (row + 1 < rows) {
                below = particleMatrix[row + 1][col];
            }
            if (row + 1 < rows && col - 1 > 0) {
                bottomLeft = particleMatrix[row + 1][col - 1];
            }
            if (row + 1 < rows && col + 1 < cols) {
                bottomRight = particleMatrix[row + 1][col + 1];
            }
            if (below != nullptr || row == rows - 1) {
                double dx, dy;
                Converter::getInstance()->getRelativeDeltas(PARTICLE_WIDTH, PARTICLE_WIDTH, &dx, &dy);
                if (bottomLeft != nullptr && bottomRight != nullptr || row == rows - 1) {
                    double rx1, ry1;
                    snapToGrid(particle->position.x, particle->position.y, &rx1, &ry1);
                    particle->position.x = rx1;
                    particle->position.y = ry1;
                    particle->inAir= false;
                    particleMatrix[row][col] = particle;
                    particle->speed.y = 0.0f;
                } else if (bottomLeft == nullptr && bottomRight == nullptr) {
                    // go a random direction
                    particle->position.x -= dx;
                    particle->position.y -= dy/2;
                    double rx1, ry1;
                    snapToGrid(particle->position.x, particle->position.y, &rx1, &ry1);
                    particle->position.x = rx1;
                    particle->position.y = ry1;
                } else if (bottomLeft == nullptr) {
                    // go left
                    particle->position.x -= dx;
                    particle->position.y -= dy/2;
                    double rx1, ry1;
                    snapToGrid(particle->position.x, particle->position.y, &rx1, &ry1);
                    particle->position.x = rx1;
                    particle->position.y = ry1;
                } else {
                    // go right
                    particle->position.x += dx;
                    particle->position.y -= dy/2;
                    double rx1, ry1;
                    snapToGrid(particle->position.x, particle->position.y, &rx1, &ry1);
                    particle->position.x = rx1;
                    particle->position.y = ry1;
                }
            } else {
                particle->speed.y -= GRAVITY * dt;
                particle->position += particle->speed * dt;
            }
        } else {
            particle->inAir = false;
        }

    }
}

void ParticleEngine::update(float dt) {
    for(auto &particle : particles) {
        switch (particle->type) {
            case SAND:
                updateSand(particle, dt);
                break;
            default:
                break;
        }
    }
}


std::vector<float> sandColor {
        194 / 255.0, 178 / 255.0, 128 / 255.0,
        194 / 255.0, 178 / 255.0, 128 / 255.0,
        194 / 255.0, 178 / 255.0, 128 / 255.0,
        194 / 255.0, 178 / 255.0, 128 / 255.0,
};

void ParticleEngine::render() {

    glUseProgram(program);
    glBindVertexArray(vao);
    double dx, dy;
    Converter::getInstance()->getRelativeDeltas(PARTICLE_WIDTH, PARTICLE_WIDTH, &dx, &dy);
    for (auto &particle : particles) {
        std::vector<float> vertexData {
                static_cast<float>(particle->position.x),  static_cast<float>(particle->position.y), 0.0f,
                static_cast<float>(particle->position.x + dx), static_cast<float>(particle->position.y), 0.0f,
                static_cast<float>(particle->position.x + dx), static_cast<float>(particle->position.y - dy), 0.0f,
                static_cast<float>(particle->position.x),  static_cast<float>(particle->position.y - dy), 0.0f
        };
        glBindBuffer(GL_ARRAY_BUFFER, vbo0);
        glBufferData(GL_ARRAY_BUFFER,sizeof(vertexData[0]) * vertexData.size(),vertexData.data(),GL_STATIC_DRAW);
        GLint posLocation = glGetAttribLocation(program, "position");
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3,GL_FLOAT,GL_FALSE,0,nullptr);

        std::vector<float> colorData;

        switch (particle->type) {
            case SAND:
                colorData = sandColor;
                break;
            default:
                colorData = sandColor;
        }

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

    int width, height;
    Converter::getInstance()->getWindowSize(&width, &height);
    rows = (int) (height / PARTICLE_WIDTH);
    cols = (int) (width / PARTICLE_WIDTH);
    printf("rows:%d\tcols:%d\n", rows, cols);
    particleMatrix = std::vector<std::vector<Particle*>>(rows,std::vector<Particle*>(cols));


}

void ParticleEngine::destroy() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo0);
    glDeleteBuffers(1, &vbo1);
}

void ParticleEngine::snapToGrid(double rx, double ry, double *rx1, double *ry1) {
    int row, col;
    getRowCol(rx, ry, &row, &col);
    double px = col * PARTICLE_WIDTH;
    double py = row * PARTICLE_WIDTH;
    Converter::getInstance()->convertPixelsToRelative(px, py, rx1, ry1);
}

void ParticleEngine::getRowCol(double rx, double ry, int* row, int* col) {
    double px, py;
    Converter::getInstance()->convertRelativeToPixels(rx, ry, &px, &py);
    if (col != nullptr) {
        *col = (int)round(px / PARTICLE_WIDTH);
    }
    if (row != nullptr) {
        *row = (int)round(py / PARTICLE_WIDTH);
    }
}

void ParticleEngine::onClick(double xPos, double yPos) {
    printf("onClick\txPos: %f\tyPos:%f\n", xPos, yPos);
    double rx, ry;
    Converter::getInstance()->convertPixelsToRelative(xPos, yPos, &rx, &ry);

    double rx1, ry1;
    snapToGrid(rx, ry, &rx1, &ry1);

    particles.push_back(new Particle(glm::vec2(rx1, ry1), SAND));
}


ParticleEngine* ParticleEngine::instancePtr = nullptr;
