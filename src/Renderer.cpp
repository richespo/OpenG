//
//  Renderer.cpp
//  OpenG
//
//  Created by Richard Esposito on 4/21/25.
//
#include "Renderer.h"
#include <iostream>
#include <assert.h> 


void GLClearError()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLLogCall(const char* function, const char* file, int line)
{
    while (GLenum error = glGetError())
    {
        std::cout << "openGL error (" << error << " ): " << function <<
            " " << file << ":" << line <<  std::endl;
        return false;
    }
    return true;
}

void Renderer::Clear() 
{
    GLCall(glClear( GL_COLOR_BUFFER_BIT ));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
    shader.Bind();
    va.Bind();
    ib.Bind();
    GLCall(glDrawElements( GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, nullptr ));
    
}


