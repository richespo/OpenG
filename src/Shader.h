//
//  Shader.h
//  OpenG
//
//  Created by Richard Esposito on 4/22/25.
//
#pragma once
#include <string>
#include <unordered_map>

struct ShaderProgramSource
{
    std::string VertexSource;
    std::string FragmentSource;
};


class Shader
{
private:
    std::string m_filepath;
    unsigned int m_RendererID;
    std::unordered_map<std::string, int> m_UniformLocationCache;
     
    //cache for uniforms
    
public:
    
    Shader(const std::string& filepath);
    ~Shader();
    
    void Bind() const;
    void Unbind() const;
    
    //set uniforms/
    void SetUniform1i(const std::string& name, int value);
    void SetUniform1f(const std::string& name, float value);
    void SetUniform4f(const std::string& name, float v0, float v1, float v3, float v4);

    unsigned int colorUniformLocation = 0;
    unsigned int textureUniformLocation = 0;
    
    
private:
    ShaderProgramSource ParseShader(const std::string& filepath);
    unsigned int CompileShader(unsigned int type, const std::string& source );
    unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
    int GetUniformLocation( const std::string& name);
    
};
