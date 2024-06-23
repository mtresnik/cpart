

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

    void convertPixelsToRelative(double px, double py, double *rx, double *ry);

    void convertRelativeToPixels(double rx, double ry, double *px, double *py);

    void getRelativeDeltas(double dpx, double dpy, double *dx, double *dy);

    void getWindowSize(int* width, int* height);

};

#endif //__MAIN___PY_CONVERTER_H
