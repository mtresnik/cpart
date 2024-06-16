

#ifndef __MAIN___PY_PARTICLE_ENGINE_H
#define __MAIN___PY_PARTICLE_ENGINE_H

#include <vector>
#include "particle.h"
#include "glad/glad.h"

class ParticleEngine {
public:
    std::vector<Particle*> particles;

    void update(float dt);
    void render();
    void init();

private:
    GLuint program;
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
};

#endif //__MAIN___PY_PARTICLE_ENGINE_H
