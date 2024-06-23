

#ifndef __MAIN___PY_PARTICLE_ENGINE_H
#define __MAIN___PY_PARTICLE_ENGINE_H

#include <vector>
#include "particle.h"
#include "glad/glad.h"

class ParticleEngine {
private:

    static ParticleEngine* instancePtr;

public:

    ParticleEngine() = default;
    std::vector<Particle*> particles;

    void update(float dt);
    void render();
    void init();
    void destroy();
    void onClick(double xPos, double yPos);

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
};

#endif //__MAIN___PY_PARTICLE_ENGINE_H
