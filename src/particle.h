

#ifndef __MAIN___PY_PARTICLE_H
#define __MAIN___PY_PARTICLE_H

#define SAND (1)
#define WATER (2)
#define WOOD (3)

#include "glm/vec2.hpp"

class Particle {
public:
    glm::vec2 position, speed;
    bool inAir;
    int type;
    bool render;
    bool isFlowing;
    bool moveLeft;
    bool moveRight;


    Particle(glm::vec2 pos, int _type) : inAir(true), position(pos), speed(0.0f, 0.0f), type(_type),
    render(true), isFlowing(false), moveLeft(false), moveRight(false) {}

};


#endif //__MAIN___PY_PARTICLE_H
