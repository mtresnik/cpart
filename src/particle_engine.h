

#ifndef __MAIN___PY_PARTICLE_ENGINE_H
#define __MAIN___PY_PARTICLE_ENGINE_H

#include <vector>
#include "particle.h"
#include "glad/glad.h"

class ParticleEngine {
private:

    static ParticleEngine* instancePtr;
    void updateSand(Particle* particle, float dt);
    void updateWater(Particle* particle, float dt);
    void snapToGrid(double rx, double ry, double *rx1, double *ry1);
    void getRowCol(double rx, double ry, int* row, int* col);
    void rowColToRelative(int row, int col, double* rx1, double *ry1);
    void moveBottomLeft(Particle* particle, double dx, double dy);
    void moveBottomRight(Particle* particle, double dx, double dy);
    void moveLeft(Particle * particle, double dx);
    void moveRight(Particle* particle, double dx);
    void placeParticle(Particle * particle, int row, int col);

public:

    ParticleEngine() = default;
    int particleType;

    void update(float dt);
    void render();
    void init();
    void destroy();
    void onClick(double xPos, double yPos);
    void generateRandom(double xPos, double yPos);

    static ParticleEngine* getInstance() {
        if (instancePtr == nullptr) {
            instancePtr = new ParticleEngine();
        }
        return instancePtr;
    }

private:
    GLuint program;
    GLuint vao;
    GLuint vbo0, vbo1;
    GLuint ebo;
    int rows, cols;
    std::vector<std::vector<Particle*>> particleMatrix;
    std::vector<Particle*> particles;
};

#endif //__MAIN___PY_PARTICLE_ENGINE_H
