
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <signal.h>
#define GLEW_STATIC
#include <GL/glew.h>
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
#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"
#define GL_SILENCE_DEPRECATION


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
    
    const char* glsl_version = "#version 150";

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows

    // Set up vertex data (and buffer(s)) and attribute pointers

    GLfloat vertices[] =
    {
      // Positions              //texture coords
        0.0f,   0.0f,            0.0f, 0.0f,  // Bottom Left
       400.0f,  0.0f,            1.0f, 0.0f, // Bottom Right
       400.0f,  300.0f,          1.0f, 1.0f,  // Top
        0.0f,   300.0f,          0.0f, 1.0f
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
     
      glm::mat4 proj = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, -1.0f, 1.0f);
      glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100, 0, 0));

      
      //glm::mat4 model = glm::translate(glm::mat4(1.0f));
      //glm::mat4 mvp = proj * view * model;
      
     Shader shader("res/shaders/basic.shader");
     shader.Bind();
     shader.SetUniform4f("u_Color",  0.2f, 0.3f, 0.8f, 1.0f);
     // shader.SetUniformMat4f("u_MVP", mvp);
      
      Texture texture("res/textures/jeanlogo.png");
      texture.Bind();
      shader.SetUniform1i("u_Texture", 0);
  
      va.Unbind();
      vb.Unbind();
      ib.Unbind();
      shader.Unbind();
      
      Renderer renderer;
      glm::vec3 translation(200, 200, 0);
     
     float r = 0.0;
     float increment = 0.05f;
     
     // Setup Dear ImGui style
     ImGui::StyleColorsDark();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
     if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
     {
         style.WindowRounding = 0.0f;
         style.Colors[ImGuiCol_WindowBg].w = 1.0f;
     }
     // Setup Platform/Renderer backends
     ImGui_ImplGlfw_InitForOpenGL(window, true);
     ImGui_ImplOpenGL3_Init(glsl_version);
 
    // Game loop
    while ( !glfwWindowShouldClose( window ) )
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.)
        GLCall(glfwPollEvents( ));
    
        renderer.Clear();
    
 
        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED) != 0)
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        {
            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.
            ImGui::SliderFloat3("Translation", &translation.x, 0.0f, 800.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            ImGui::End();
        }

        glm::mat4 model = glm::translate(glm::mat4(1.0f), translation);
        glm::mat4 mvp = proj * view * model;
             
        shader.Bind();
        shader.SetUniform4f("u_Color",  r, 0.3f, 0.8f, 1.0f );
        shader.SetUniformMat4f("u_MVP", mvp);
             
        va.Bind();
        ib.Bind();
        renderer.Draw(va, ib, shader);
             
        if (r > 1.0f)
            increment = -0.05f;
        else if (r < 0)
            increment = 0.05f;
        r += increment;
 
        ImGui::Render();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);

       //glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
        // Swap the screen buffers
        GLCall(glfwSwapBuffers( window ));
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    GLCall(glfwTerminate( ));

    return EXIT_SUCCESS;
}
