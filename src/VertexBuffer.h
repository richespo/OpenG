//
//  VertexBuffer.h
//  OpenG
//
//  Created by Richard Esposito on 4/21/25.
//
#pragma once

class VertexBuffer
{
    private:
    unsigned int m_RendererID;
public:
    VertexBuffer(const void* data, unsigned int size);
    ~VertexBuffer();
    
    void Bind() const;
    void Unbind() const;
    
};
