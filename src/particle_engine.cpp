

#include "particle_engine.h"
#include "glad/glad.h"
#include "util.h"
#include "converter.h"

#define GRAVITY (0.1f)
#define PARTICLE_SPEED (0.2f)
#define PARTICLE_WIDTH (4)
#define NUM_RANDOM_PARTICLES (20)
#define BRUSH_RADIUS (10)

#include <fstream>
#include <iterator>

void ParticleEngine::updateSand(Particle* particle, float dt) {
    if (particle->inAir) {
        int row, col;
        getRowCol(particle->position.x, particle->position.y, &row, &col);
        if (row >= 0 && col >= 0 && row < rows && col < cols) {
            Particle* below = nullptr;
            Particle* left = nullptr;
            Particle* right = nullptr;
            Particle* bottomLeft = nullptr;
            Particle* bottomRight = nullptr;
            if (row + 1 < rows) {
                below = particleMatrix[row + 1][col];
            }
            if (row + 1 < rows && col - 1 >= 0) {
                bottomLeft = particleMatrix[row + 1][col - 1];
            }
            if (row + 1 < rows && col + 1 < cols) {
                bottomRight = particleMatrix[row + 1][col + 1];
            }
            if (col - 1 >= 0) {
                left = particleMatrix[row][col - 1];
            }
            if (col + 1 < cols) {
                right = particleMatrix[row][col + 1];
            }
            if (below != nullptr) {
                double dx, dy;
                Converter::getInstance()->getRelativeDeltas(PARTICLE_WIDTH, PARTICLE_WIDTH, &dx, &dy);
                if (bottomLeft != nullptr && bottomRight != nullptr || row == rows - 1) {
                    placeParticle(particle, row, col);
                } else if (bottomLeft == nullptr && bottomRight == nullptr) {
                    // go left
                    if (left == nullptr || left->type == WATER) {
                        moveBottomLeft(particle, dx, dy);
                    }
                } else if (bottomLeft == nullptr) {
                    // go left
                    if (left == nullptr || left->type == WATER) {
                        moveBottomLeft(particle, dx, dy);
                    }
                } else {
                    // go right
                    if (right == nullptr || right->type == WATER) {
                        moveBottomRight(particle, dx, dy);
                    }
                }
            } else {
                particle->speed.y -= GRAVITY * dt;
                particle->position += particle->speed * dt;
            }
        } else {
            particle->inAir = false;
            particle->render = false;
        }

    }
}

void ParticleEngine::moveBottomLeft(Particle* particle, double dx, double dy) {
    particle->position.x -= dx;
    particle->position.y -= dy/2;
    double rx1, ry1;
    snapToGrid(particle->position.x, particle->position.y, &rx1, &ry1);
    particle->position.x = rx1;
    particle->position.y = ry1;
}

void ParticleEngine::moveBottomRight(Particle* particle, double dx, double dy) {
    particle->position.x += dx;
    particle->position.y -= dy/2;
    double rx1, ry1;
    snapToGrid(particle->position.x, particle->position.y, &rx1, &ry1);
    particle->position.x = rx1;
    particle->position.y = ry1;
}

void ParticleEngine::moveLeft(Particle * particle, double dx) {
    particle->position.x -= dx;
    double rx1, ry1;
    snapToGrid(particle->position.x, particle->position.y, &rx1, &ry1);
    particle->position.x = rx1;
    particle->position.y = ry1;
}

void ParticleEngine::moveRight(Particle* particle, double dx) {
    particle->position.x += dx;
    double rx1, ry1;
    snapToGrid(particle->position.x, particle->position.y, &rx1, &ry1);
    particle->position.x = rx1;
    particle->position.y = ry1;
}

void ParticleEngine::placeParticle(Particle * particle, int row, int col) {
    double rx1, ry1;
    snapToGrid(particle->position.x, particle->position.y, &rx1, &ry1);
    particle->position.x = rx1;
    particle->position.y = ry1;
    particle->inAir= false;
    particleMatrix[row][col] = particle;
    particle->speed.y = 0.0f;
}

void ParticleEngine::updateWater(Particle* particle, float dt) {

    int row, col;
    getRowCol(particle->position.x, particle->position.y, &row, &col);

    if (row > 0 && col > 0 && row < rows - 1 && col < cols - 1) {
        Particle* below = nullptr;
        Particle* left = nullptr;
        Particle* right = nullptr;
        if (row + 1 < rows) {
            below = particleMatrix[row + 1][col];
        }
        if (col - 1 >= 0) {
            left = particleMatrix[row][col - 1];
        }
        if (col + 1 < cols) {
            right = particleMatrix[row][col + 1];
        }
        double dx, dy;
        Converter::getInstance()->getRelativeDeltas(PARTICLE_WIDTH, PARTICLE_WIDTH, &dx, &dy);
        if (!particle->isFlowing) {
            if (below != nullptr) {
                if (left == nullptr || right == nullptr) {
                    particle->isFlowing = true;
                } else {
                    placeParticle(particle, row, col);
                }
            } else {
                particle->speed.y -= GRAVITY * dt;
                particle->position += particle->speed * dt;
            }
        } else {
            if (below == nullptr) {
                particle->isFlowing = false;
            } else {
                if (!particle->moveLeft && !particle->moveRight) {
                    // Pick a direction to start moving
                    float r = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                    if (r < 0.5) {
                        particle->moveLeft = true;
                    } else {
                        particle->moveRight = true;
                    }
                }
                if (left != nullptr && particle->moveLeft && right == nullptr) {
                    particle->moveRight = true;
                    particle->moveLeft = false;
                } else if (right != nullptr && particle->moveRight && left == nullptr) {
                    particle->moveLeft = true;
                    particle->moveRight = false;
                }
                if (left == nullptr && particle->moveLeft) {
                    moveLeft(particle, dx);
                    particleMatrix[row][col] = nullptr;
                    getRowCol(particle->position.x, particle->position.y, &row, &col);
                    placeParticle(particle, row, col);
                } else if (right == nullptr && particle->moveRight) {
                    moveRight(particle, dx);
                    particleMatrix[row][col] = nullptr;
                    getRowCol(particle->position.x, particle->position.y, &row, &col);
                    placeParticle(particle, row, col);
                } else if (left != nullptr && right != nullptr) {
                    particle->isFlowing = false;
                }
            }
        }
    } else {
        particle->inAir = false;
        particle->render = false;
    }
}

void ParticleEngine::update(float dt) {
    for(auto &particle : particles) {
        switch (particle->type) {
            case SAND:
                updateSand(particle, dt);
                break;
            case WATER:
                updateWater(particle, dt);
                break;
            case WOOD:
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

std::vector<float> waterColor {
        15 / 255.0, 94 / 255.0, 156 / 255.0,
        15 / 255.0, 94 / 255.0, 156 / 255.0,
        15 / 255.0, 94 / 255.0, 156 / 255.0,
        15 / 255.0, 94 / 255.0, 156 / 255.0,
};

std::vector<float> woodColor {
        161 / 255.0, 102 / 255.0, 47 / 255.0,
        161 / 255.0, 102 / 255.0, 47 / 255.0,
        161 / 255.0, 102 / 255.0, 47 / 255.0,
        161 / 255.0, 102 / 255.0, 47 / 255.0,
};

void ParticleEngine::render() {

    glUseProgram(program);
    glBindVertexArray(vao);
    double dx, dy;
    Converter::getInstance()->getRelativeDeltas(PARTICLE_WIDTH, PARTICLE_WIDTH, &dx, &dy);
    for (auto &particle : particles) {
        if (!particle->render) {
            continue;
        }
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
            case WATER:
                colorData = waterColor;
                break;
            case WOOD:
                colorData = woodColor;
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
    particleType = SAND;

    for (int row = 0; row < rows; ++row) {
        double rx, ry;
        rowColToRelative(row, 0, &rx, &ry);
        particleMatrix[row][0] = new Particle(glm::vec2(rx, ry), WOOD);
        particles.push_back(particleMatrix[row][0]);
    }

    for (int row = 0; row < rows; ++row) {
        double rx, ry;
        rowColToRelative(row, cols - 1, &rx, &ry);
        particleMatrix[row][cols - 1] = new Particle(glm::vec2(rx, ry), WOOD);
        particles.push_back(particleMatrix[row][cols - 1]);
    }

    for (int col = 0; col < cols; ++col) {
        double rx, ry;
        rowColToRelative(0, col, &rx, &ry);
        particleMatrix[0][col] = new Particle(glm::vec2(rx, ry), WOOD);
        particles.push_back(particleMatrix[0][col]);
    }

    for (int col = 0; col < cols; ++col) {
        double rx, ry;
        rowColToRelative(rows - 1, col, &rx, &ry);
        particleMatrix[rows - 1][col] = new Particle(glm::vec2(rx, ry), WOOD);
        particles.push_back(particleMatrix[rows - 1][col]);
    }

}

void ParticleEngine::destroy() {
    glDeleteProgram(program);
    glDeleteBuffers(1, &vbo0);
    glDeleteBuffers(1, &vbo1);
}

void ParticleEngine::snapToGrid(double rx, double ry, double *rx1, double *ry1) {
    int row, col;
    getRowCol(rx, ry, &row, &col);
    rowColToRelative(row, col, rx1, ry1);
}

void ParticleEngine::rowColToRelative(int row, int col, double* rx1, double *ry1) {
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
    double rx, ry;
    Converter::getInstance()->convertPixelsToRelative(xPos, yPos, &rx, &ry);

    double rx1, ry1;
    snapToGrid(rx, ry, &rx1, &ry1);

    particles.push_back(new Particle(glm::vec2(rx1, ry1), particleType));
}

void ParticleEngine::generateRandom(double xPos, double yPos) {
    for (int i = 0; i < NUM_RANDOM_PARTICLES; ++i) {
        float r = 2.0f * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 1.0f;
        float s = 2.0f * (static_cast <float> (rand()) / static_cast <float> (RAND_MAX)) - 1.0f;
        double rx, ry;
        Converter::getInstance()->convertPixelsToRelative(xPos + BRUSH_RADIUS * PARTICLE_WIDTH * r, yPos + BRUSH_RADIUS * PARTICLE_WIDTH * s, &rx, &ry);

        double rx1, ry1;
        snapToGrid(rx, ry, &rx1, &ry1);
        particles.push_back(new Particle(glm::vec2(rx1, ry1), particleType));
    }
}


ParticleEngine* ParticleEngine::instancePtr = nullptr;
