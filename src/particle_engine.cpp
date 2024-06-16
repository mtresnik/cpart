

#include "particle_engine.h"
#include "glad/glad.h"
#include "util.h"

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

}

void ParticleEngine::init() {
    program = createShaderProgram("vertex.glsl", "fragment.glsl");

    particles.push_back(new Particle(glm::vec2(0.0, 1.0), 4));
}
