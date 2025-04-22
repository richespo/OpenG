//
//  Renderer.cpp
//  OpenG
//
//  Created by Richard Esposito on 4/21/25.
//
#include "Renderer.h"
#include <iostream>

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
