

#ifndef __MAIN___PY_CONVERTER_H
#define __MAIN___PY_CONVERTER_H

#include <cstdio>
#include "GLFW/glfw3.h"

class Converter {
private:
    static Converter* instancePtr;

    Converter() = default;

    GLFWwindow* window;

public:
    void init(GLFWwindow* _window) {
        this->window = _window;
    }

    static Converter* getInstance() {
        if (instancePtr == nullptr) {
            instancePtr = new Converter();
        }
        return instancePtr;
    }

    void convert(double px, double py, double *rx, double *ry) {
        if (rx) {
            *rx = 0;
        }
        if (ry) {
            *ry = 0;
        }
        if (window != nullptr) {
            int width, height;
            glfwGetWindowSize(window, &width, &height);
            if (rx != nullptr) {
                *rx = 2 * (px / width) - 1;
                printf("rx: %f\n", *rx);
            }
            if (ry != nullptr) {
                *ry = -2 * (py / height) + 1;
                printf("ry: %f\n", *ry);
            }
        }
    }

};

#endif //__MAIN___PY_CONVERTER_H
