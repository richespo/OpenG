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
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define assert(x) if ((!x)) raise(SIGTRAP);
#define GLCall(x) GLClearError();\
    x;\
assert(GLLogCall(#x, __FILE__, __LINE__))


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
             -0.5f, -0.5f, 0.0f, 0.0f,  // Bottom Right
              0.5f, -0.5f, 1.0f, 0.0f, // Bottom Left
              0.5f,  0.5f, 1.0f, 1.0f,  // Top
             -0.5f,  0.5f, 0.0f, 1.0f
         };
        
        unsigned int indices[] =
        {
            0,1,2,
            2,3,0
        };
        
         GLCall(glEnable(GL_BLEND));
         GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
         
        VertexArray va;
        VertexBuffer vb(vertices, 4 * 4 * sizeof(float));
        VertexBufferLayout(layout);
        layout.Push<float>(2);
        layout.Push<float>(2);
        va.AddBuffer(vb, layout);
         
         IndexBuffer ib(indices, 6);
        
         glm::mat4 proj = glm::ortho(-2.0f, 2.0f, -1.5f, 1.5f, -1.0f, 1.0f);
         
         
        Shader shader("res/shaders/basic.shader");
        shader.Bind();
        shader.SetUniform4f("u_Color",  0.2f, 0.3f, 0.8f, 1.0f);
        shader.SetUniformMat4f("u_MVP", proj); 
         
         Texture texture("res/textures/jeanlogo.png");
         texture.Bind();
         shader.SetUniform1i("u_Texture", 0);
         
         va.Unbind();
         vb.Unbind();
         ib.Unbind();
         shader.Unbind();
         
         Renderer renderer;
         
        float r = 0.0;
        float increment = 0.05f;
        // Game loop
        while ( !glfwWindowShouldClose( window ) )
        {
            // Check if any events have been activiated (key pressed, mouse moved etc.)
            GLCall(glfwPollEvents( ));
            
            renderer.Clear();
            shader.Bind();
            shader.SetUniform4f("u_Color",  r, 0.3f, 0.8f, 1.0f );
            
            va.Bind();
            ib.Bind();
            renderer.Draw(va, ib, shader);
            
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

