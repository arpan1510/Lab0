#include <windows.h>

//Imgui headers for UI
#include "imgui.h"
#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

#include "DebugCallback.h"
#include "InitShader.h"    //Functions for loading shaders from text files
#include "LoadMesh.h"      //Functions for creating OpenGL buffers from mesh files
#include "LoadTexture.h"   //Functions for creating OpenGL textures from image files

namespace window
{
   const char* const title = USER_NAME " " PROJECT_NAME; //defined in project settings
   int size[2] = {1024, 1024};
   float clear_color[4] = {0.35f, 0.35f, 0.35f, 1.0f};
}

namespace scene
{
   const std::string asset_dir = "assets/";
   const std::string mesh_name = "Amago0.obj";
   const std::string tex_name = "AmagoT.bmp";

   MeshData mesh;
   GLuint tex_id = -1; //Texture map for mesh

   float angle = 0.0f;
   float scale = 1.0f;

   bool isClearScreen = true;
   bool isEnableDepthTesting = true;
   int clicked = 0;
   float colorMultiplier = 1;
   int waveSpeed = 0;
   float scaleNonuniform[3] = { 1.0f,1.0f,1.0f};
   bool isRotationAutomatic = false;
   float tPosition = 0;
   int multOrder = 0;
   float lookatAtt[3] = { 0.0f,0.0f,1.0f };
   float fov= glm::pi<float>() / 2;
   float nearz=0.1f;
   float farz=3.0f;

   const std::string shader_dir = "shaders/";
   const std::string vertex_shader("lab2_vs.glsl");
   const std::string fragment_shader("lab2_fs.glsl");

   GLuint shader = -1;
}
//For an explanation of this program's structure see https://www.glfw.org/docs/3.3/quick.html 


//Draw the ImGui user interface
void draw_gui(GLFWwindow* window)
{
   //Begin ImGui Frame
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();

   //Draw Gui
   ImGui::Begin("Debug window");
   if (ImGui::Button("Quit"))
   {
      glfwSetWindowShouldClose(window, GLFW_TRUE);
   }
   //Lab 2: Uncomment these 
   
   ImGui::SliderFloat("Rotation angle", &scene::angle, -glm::pi<float>(), +glm::pi<float>());
   ImGui::SliderFloat("Scale", &scene::scale, -10.0f, +10.0f);

   ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
   ImGui::End();


   ImGui::Begin("Arpan Prajapati");
   ImGui::Text("Lab 2.1");
   ImGui::Checkbox("Enable Screen Clearing", &scene::isClearScreen);
   ImGui::Checkbox("Enable Depth Testing", &scene::isEnableDepthTesting);
   ImGui::ColorEdit4("Change Background Color", window::clear_color);
   if (ImGui::Button("Reset Rotation & Scale"))
   {
       scene::angle = 0.0f;
       scene::scale = 1.0f;
   }
   ImGui::SliderFloat("Brightness", &scene::colorMultiplier, 0, 2);
   ImGui::RadioButton("Static Fish", &scene::waveSpeed, 0); ImGui::SameLine();
   ImGui::RadioButton("Animated Fish", &scene::waveSpeed, 1);
   ImGui::Text("Lab 2.1");
   ImGui::SliderFloat3("Non-Uniform Scaling", scene::scaleNonuniform, -2.0f, 2.0f);
   ImGui::Checkbox("Automatic Rotation", &scene::isRotationAutomatic);
   ImGui::SliderFloat("Translate", &scene::tPosition, -1.0f, +1.0f);
   ImGui::RadioButton("M=T*R*S", &scene::multOrder, 0); ImGui::SameLine();
   ImGui::RadioButton("M=R*T*S", &scene::multOrder, 1);
   ImGui::Text("Lab 2.3");
   ImGui::SliderFloat3("Camera Position", scene::lookatAtt, -1.0f, 1.0f);
   ImGui::SliderFloat("fov", &scene::fov, 0, glm::pi<float>());
   ImGui::SliderFloat("near clipping distance", &scene::nearz, 0.1f, 3.0f);
   ImGui::SliderFloat("far clipping distance", &scene::farz, 0.1f, 5.0f);

   ImGui::End();


   static bool show_test = true;
   if(show_test)
   {
      ImGui::ShowDemoWindow(&show_test);
   }

   //End ImGui Frame
   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


// This function gets called every time the scene gets redisplayed
void display(GLFWwindow* window)
{
   //Clear the screen to the color previously specified in the glClearColor(...) call.
    if (scene::isClearScreen)
    {
        glClearColor(window::clear_color[0], window::clear_color[1], window::clear_color[2], window::clear_color[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    if (scene::isEnableDepthTesting)
    {
        glEnable(GL_DEPTH_TEST);
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }

    if (scene::isRotationAutomatic)
    {
        scene::angle = scene::angle + glm::pi<float>()/ ImGui::GetIO().Framerate;
    }
    
    glm::mat4 T = glm::translate(glm::vec3(scene::tPosition, 0.0f, 0.0f));
    glm::mat4 R = glm::rotate(scene::angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 S = glm::scale(glm::vec3(scene::scaleNonuniform[0] * scene::mesh.mScaleFactor, scene::scaleNonuniform[1] * scene::mesh.mScaleFactor, scene::scaleNonuniform[2] * scene::mesh.mScaleFactor));
    
    glm::mat4 M = T * R * S;
    if (scene::multOrder == 0)
    {
        M = T * R * S;
    }
    else
    {
        M = R * T * S;
    }
   glm::mat4 V = glm::lookAt(glm::vec3(scene::lookatAtt[0], scene::lookatAtt[1], scene::lookatAtt[2]), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
   glm::mat4 P = glm::perspective(scene::fov, 1.0f, scene::nearz, scene::farz);

   glUseProgram(scene::shader);

   glActiveTexture(GL_TEXTURE0);
   glBindTexture(GL_TEXTURE_2D, scene::tex_id);
   int tex_loc = glGetUniformLocation(scene::shader, "diffuse_tex");
   if (tex_loc != -1)
   {
      glUniform1i(tex_loc, 0); // we bound our texture to texture unit 0
   }

   //Get location for shader uniform variable
   int PVM_loc = glGetUniformLocation(scene::shader, "PVM");
   if (PVM_loc != -1)
   {
      glm::mat4 PVM = P * V * M;
      //Set the value of the variable at a specific location
      glUniformMatrix4fv(PVM_loc, 1, false, glm::value_ptr(PVM));
   }

   glBindVertexArray(scene::mesh.mVao);
   scene::mesh.DrawMesh();

   draw_gui(window);

   // Swap front and back buffers
   glfwSwapBuffers(window);
}

void idle()
{
   float time_sec = static_cast<float>(glfwGetTime());

   //Pass time_sec value to the shaders
   int time_loc = glGetUniformLocation(scene::shader, "time");
   if (time_loc != -1)
   {
      glUniform1f(time_loc, time_sec);
   }

   //Pass colorMultiplier value to the shaders
   int cm_loc = glGetUniformLocation(scene::shader, "cm");
   if (cm_loc != -1)
   {
       glUniform1f(cm_loc, scene::colorMultiplier);
   }

   int speed_loc = glGetUniformLocation(scene::shader, "speed");
   if (speed_loc != -1)
   {
       if (scene::waveSpeed == 1) {
           glUniform1f(speed_loc, 5.0);
       }
       else
       {
           glUniform1f(speed_loc, 0.0);
       }
       
   }
}

void reload_shader()
{
   std::string vs = scene::shader_dir + scene::vertex_shader;
   std::string fs = scene::shader_dir + scene::fragment_shader;

   GLuint new_shader = InitShader(vs.c_str(), fs.c_str());

   if (new_shader == -1) // loading failed
   {
      glClearColor(1.0f, 0.0f, 1.0f, 0.0f); //change clear color if shader can't be compiled
   }
   else
   {
      glClearColor(window::clear_color[0], window::clear_color[1], window::clear_color[2], window::clear_color[3]);

      if (scene::shader != -1)
      {
         glDeleteProgram(scene::shader);
      }
      scene::shader = new_shader;
   }
}

//This function gets called when a key is pressed
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   std::cout << "key : " << key << ", " << char(key) << ", scancode: " << scancode << ", action: " << action << ", mods: " << mods << std::endl;

   if (action == GLFW_PRESS)
   {
      switch (key)
      {
      case 'r':
      case 'R':
         reload_shader();
         break;

      case GLFW_KEY_ESCAPE:
         glfwSetWindowShouldClose(window, GLFW_TRUE);
         break;
      }
   }
}

//This function gets called when the mouse moves over the window.
void mouse_cursor(GLFWwindow* window, double x, double y)
{
   //std::cout << "cursor pos: " << x << ", " << y << std::endl;
}

//This function gets called when a mouse button is pressed.
void mouse_button(GLFWwindow* window, int button, int action, int mods)
{
   //std::cout << "button : "<< button << ", action: " << action << ", mods: " << mods << std::endl;
}

//Initialize OpenGL state. This function only gets called once.
void init()
{
   glewInit();
   RegisterDebugCallback();

   std::ostringstream oss;
   //Get information about the OpenGL version supported by the graphics driver.	
   oss << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   oss << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   oss << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   oss << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;

   //Output info to console
   std::cout << oss.str();

   //Output info to file
   std::fstream info_file("info.txt", std::ios::out | std::ios::trunc);
   info_file << oss.str();
   info_file.close();

   //Set the color the screen will be cleared to when glClear is called
   glClearColor(window::clear_color[0], window::clear_color[1], window::clear_color[2], window::clear_color[3]);

   
   

   reload_shader();
   scene::mesh = LoadMesh(scene::asset_dir + scene::mesh_name);
   scene::tex_id = LoadTexture(scene::asset_dir + scene::tex_name);
}


// C++ programs start executing in the main() function.
int main(void)
{
   GLFWwindow* window;

   // Initialize the library
   if (!glfwInit())
   {
      return -1;
   }

#ifdef _DEBUG
   glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

   // Create a windowed mode window and its OpenGL context
   window = glfwCreateWindow(window::size[0], window::size[1], window::title, NULL, NULL);
   if (!window)
   {
      glfwTerminate();
      return -1;
   }

   //Register callback functions with glfw. 
   glfwSetKeyCallback(window, keyboard);
   glfwSetCursorPosCallback(window, mouse_cursor);
   glfwSetMouseButtonCallback(window, mouse_button);

   // Make the window's context current
   glfwMakeContextCurrent(window);

   init();

   // New in Lab 2: Init ImGui
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGui_ImplGlfw_InitForOpenGL(window, true);
   ImGui_ImplOpenGL3_Init("#version 150");

   // Loop until the user closes the window 
   while (!glfwWindowShouldClose(window))
   {
      idle();
      display(window);

      // Poll for and process events 
      glfwPollEvents();
   }

   // New in Lab 2: Cleanup ImGui
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();

   glfwTerminate();
   return 0;
}