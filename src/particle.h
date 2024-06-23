

#ifndef __MAIN___PY_PARTICLE_H
#define __MAIN___PY_PARTICLE_H

#define SAND (0)
#define WATER (1)

#include "glm/vec2.hpp"

class Particle {
public:
    glm::vec2 position, speed;
    bool inAir;
    int type;


    Particle(glm::vec2 pos, int _type) : inAir(true), position(pos), speed(0.0f, 0.0f), type(_type) {}

};


#endif //__MAIN___PY_PARTICLE_H
