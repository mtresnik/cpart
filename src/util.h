

#ifndef __MAIN___PY_UTIL_H
#define __MAIN___PY_UTIL_H

#include "glad/glad.h"
#include <string>

GLuint createShaderProgram(const std::string& vertexShaderPath,
                           const std::string& fragmentShaderPath);
#endif //__MAIN___PY_UTIL_H
