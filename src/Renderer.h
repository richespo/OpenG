//
//  Renderer.h
//  OpenG
//
//  Created by Richard Esposito on 4/21/25.
//
#pragma once

#include <GL/glew.h>
#include <assert.h>

#define assert(x) if ((!x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
assert(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);
