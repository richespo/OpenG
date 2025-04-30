//
//  Renderer.h
//  OpenG
//
//  Created by Richard Esposito on 4/21/25.
//
#pragma once

#include <GL/glew.h>
#include <assert.h>
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

//#define assert(x) if (!(x)) raise(SIGABRT);
#define GLCall(x) GLClearError();\
    x;\
assert(GLLogCall(#x, __FILE__, __LINE__))

void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);

 class Renderer
{
    
public:
    void Clear();
    void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
    
};
