#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <signal.h>
// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include  <GLFW/glfw3.h>
#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
 


static ShaderProgramSource ParseShader(const std::string& filepath)
{
    enum class ShaderType
    {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };
    std::ifstream stream(filepath);
    std::string line;
    ShaderType type = ShaderType::NONE;
    std::stringstream ss[2];
    while (getline(stream, line))
    {
       if (line.find("#shader") != std::string::npos)
       {
           if (line.find("vertex") != std::string::npos)
               type = ShaderType::VERTEX;
           else if (line.find("fragment") != std::string::npos)
               type = ShaderType::FRAGMENT;
       }
       else
       {
           ss[(int)type] << line << std::endl;
       }
    }
    return { ss[0].str(), ss[1].str()};
}
 
static unsigned int CompileShader(unsigned int type, const std::string& source )
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);
                      
    int result;
    GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
    if (result == GL_FALSE)
    {
        int length;
        GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
        char* message = (char*)alloca(length * sizeof(char)) ;
        GLCall(glGetShaderInfoLog(id, length, &length, message));
        std::cout << "Failed to compile shader" << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << std::endl;
        std::cout << message << std::endl;
        GLCall(glDeleteShader(id));
        return 0;
    }
    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
                     
    GLCall( glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
                     
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    return program;
}


// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 600;

// The MAIN function, from here we start the application and run the game loop
int main( )
{
    // Init GLFW
    glfwInit( );
    
    // Set all the required options for GLFW
    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );
    glfwWindowHint( GLFW_RESIZABLE, GL_FALSE );
    
    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow( WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr );
    
    GLCall(glfwSwapInterval(1));
    
    int screenWidth, screenHeight;
    GLCall(glfwGetFramebufferSize( window, &screenWidth, &screenHeight ));
    
    if ( nullptr == window )
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        GLCall(glfwTerminate( ));
        return EXIT_FAILURE;
    }
    
    GLCall(glfwMakeContextCurrent( window ));
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if ( GLEW_OK != glewInit( ) )
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return EXIT_FAILURE;
    }
    
    // Define the viewport dimensions
    GLCall(glViewport( 0, 0, screenWidth, screenHeight ));
    
    /*
    std::cout << "VERTEX" << std::endl;
    std::cout << source.VertexSource << std::endl;
    std::cout << "FRAGMENT" << std::endl;
    std::cout << source.FragmentSource << std::endl;
    */
     {
        // Set up vertex data (and buffer(s)) and attribute pointers
        GLfloat vertices[] =
        {
            // Positions
            -0.5f, -0.5f,  // Bottom Right
            0.5f, -0.5f,  // Bottom Left
            0.5f,  0.5f,  // Top
            -0.5f,  0.5f
        };
        
        unsigned int indices[] =
        {
            0,1,2,
            2,3,0
        };
        
        VertexArray va;
        VertexBuffer vb(vertices, 4 * 2 * sizeof(float));
        VertexBufferLayout(layout);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
        
        IndexBuffer ib(indices, 6);
        
        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color",  0.2f, 0.3f, 0.8f, 1.0f);
         
         va.Unbind();
         vb.Unbind();
         ib.Unbind();
         shader.Unbind();
         
        float r = 0.0;
        float increment = 0.05f;
        // Game loop
        while ( !glfwWindowShouldClose( window ) )
        {
            // Check if any events have been activiated (key pressed, mouse moved etc.)
            GLCall(glfwPollEvents( ));
            
            GLCall(glClear( GL_COLOR_BUFFER_BIT ));
            shader.Bind();
            shader.SetUniform4f("u_Color",  r, 0.3f, 0.8f, 1.0f );
            
            va.Bind();
            ib.Bind();
            GLCall(glDrawElements( GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr ));
            
            if (r > 1.0f)
                increment = -0.05f;
            else if (r < 0)
                increment = 0.05f;
            
            r += increment;
            
            // Swap the screen buffers
            GLCall(glfwSwapBuffers( window ));
        }
    }
    // Terminate GLFW, clearing any resources allocated by GLFW.
    GLCall(glfwTerminate( ));
    
    return EXIT_SUCCESS;
}

