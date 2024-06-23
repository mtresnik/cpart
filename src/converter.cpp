#include "converter.h"

Converter* Converter::instancePtr = nullptr;

void Converter::getWindowSize(int* width, int* height) {
    glfwGetWindowSize(window, width, height);
}

void Converter::getRelativeDeltas(double dpx, double dpy, double *dx, double *dy) {
    if (dx) {
        *dx = 0;
    }
    if (dy) {
        *dy = 0;
    }
    if (window != nullptr) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        if (dx != nullptr) {
            *dx = 2 * (dpx / width);
        }
        if (dy != nullptr) {
            *dy = 2 * (dpy / height);
        }
    }
}

void Converter::convertPixelsToRelative(double px, double py, double *rx, double *ry) {
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

void Converter::convertRelativeToPixels(double rx, double ry, double *px, double *py) {
    if (px) {
        *px = 0;
    }
    if (py) {
        *py = 0;
    }
    if (window != nullptr) {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        if (px != nullptr) {
            *px = (rx + 1) * width / 2;
        }
        if (py != nullptr) {
            *py = (ry - 1) * height / -2;
        }
    }
}

