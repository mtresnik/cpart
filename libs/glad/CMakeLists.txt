cmake_minimum_required(VERSION 3.5)
project(glad)

add_library(glad include/glad/glad.h include/glad/khrplatform.h src/glad.c)
target_include_directories(glad PUBLIC include/)