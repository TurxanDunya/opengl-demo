#pragma once

#include <glew.h>

#define ASSERT(x) if (!(x)) __debugbreak(); // __ means this method belong to MSVC, won't work on Clang or gcc
#define GLCall(x) GLClearError();\
    x;\
    ASSERT(GLLogCall(#x, __FILE__, __LINE__)) // # - means make string anything passed here

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);