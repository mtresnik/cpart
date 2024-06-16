

#include <string>
#include <sstream>
#include <fstream>
#include "util.h"
#include "glad/glad.h"

GLuint createShaderProgram(const std::string& vertexShaderPath,
                           const std::string& fragmentShaderPath) {
    // Load the vertex shader source.
    std::stringstream ss{ };
    std::string vertexShaderSource{ };
    std::string fragmentShaderSource{ };
    std::ifstream file{ vertexShaderPath };
    if (file.is_open()) {
        ss << file.rdbuf();
        vertexShaderSource = ss.str();
        file.close();
    }
    // Clear the stringstream and load the fragment shader source.
    ss.str(std::string{ });
    file.open(fragmentShaderPath);
    if (file.is_open()) {
        ss << file.rdbuf();
        fragmentShaderSource = ss.str();
        file.close();
    }
    // Create the program.
    GLuint program = glCreateProgram();
    // Create the shaders.
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // Now we can load the shader source into the shader objects and compile them.
    // Because glShaderSource() wants a const char* const*,
    // we must first create a const char* and then pass the reference.
    const char* cVertexSource = vertexShaderSource.c_str();
    glShaderSource(vertexShader,     // shader
                   1,                // number of strings
                   &cVertexSource,   // strings
                   nullptr);         // length of strings (nullptr for 1)
    glCompileShader(vertexShader);
    // Now we have to do the same for the fragment shader.
    const char* cFragmentSource = fragmentShaderSource.c_str();
    glShaderSource(fragmentShader, 1, &cFragmentSource, nullptr);
    glCompileShader(fragmentShader);
    // After attaching the source and compiling the shaders,
    // we attach them to the program;
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    // After linking the shaders we should detach and delete
    // them to prevent memory leak.
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // With everything done we can return the completed program.
    return program;
}