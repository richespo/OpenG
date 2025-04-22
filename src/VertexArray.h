//
//  VertexArray.h
//  OpenG
//
//  Created by Richard Esposito on 4/22/25.
//
#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

class VertexArray
{
private:
    unsigned int m_RendererID;
    
public:
    VertexArray();
    ~VertexArray();
    
    void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    
    void Bind() const;
    void Unbind() const;
    
};

