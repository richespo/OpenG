//
//  IndexBuffer.h
//  OpenG
//
//  Created by Richard Esposito on 4/21/25.
//
#pragma once

class IndexBuffer
{
private:
    unsigned int m_RendererID;
    unsigned int m_Count;
public:
    IndexBuffer(const unsigned int*  data, unsigned int count);
    ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const ;
    
    inline unsigned int GetCount() const {return m_Count;}
    
};

