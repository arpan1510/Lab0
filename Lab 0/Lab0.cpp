#include <windows.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <fstream>

namespace window
{
   const char* const title = USER_NAME " " PROJECT_NAME; //defined in project settings
   int size[2] = {512, 512};
   float clear_color[4] = {1.0f, 0.0f, 0.0f, 1.0f};
}

//For an explanation of this program's structure see https://www.glfw.org/docs/3.3/quick.html 

// This function gets called every time the scene gets redisplayed
void display(GLFWwindow* window)
{
   //Clear the screen to the color previously specified in the glClearColor(...) call.
   glClear(GL_COLOR_BUFFER_BIT);

   //Here is where you would write code to draw something.

   /* Swap front and back buffers */
   glfwSwapBuffers(window);
}

//This function gets called when a key is pressed
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods)
{
   //exit if escape key is pressed
   int state = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT);
   int state1 = glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT);

   if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
   {

      glfwSetWindowShouldClose(window, GLFW_TRUE);
   }

   //Maximize Screen

   if (key == GLFW_KEY_M && action == GLFW_PRESS)
   {
       int maxWidth = GetSystemMetrics(SM_CXMAXIMIZED);
       int maxHeight = GetSystemMetrics(SM_CYMAXIMIZED);
    
       int maximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);

       if (maximized == FALSE)
       {
           glfwMaximizeWindow(window);
       }
       else
       {
           glfwRestoreWindow(window);
       }
       
   }

   //Pressing Keys

   if (((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && (action == GLFW_PRESS || action == GLFW_REPEAT)))
   {
       std::cout << "key : " << key << ", Shift Active , scancode: " << scancode << ", action: PRESS " << action << ", mods: " << mods << std::endl;
   }
   else if ((key == GLFW_KEY_LEFT_SHIFT || key == GLFW_KEY_RIGHT_SHIFT) && action == GLFW_RELEASE)
   {
       std::cout << "key : " << key << ", Shift Inactive , scancode: " << scancode << ", action: RELEASE " << action << ", mods: " << mods << std::endl;
   }
   else
   {
       if(state==GLFW_PRESS || state == GLFW_REPEAT || state1 == GLFW_PRESS || state1 == GLFW_PRESS)
       {
           std::cout << "key : " << key << ", " << char(key) << " : Shift Active, scancode: " << scancode << ", action: PRESS " << action << ", mods: " << mods << std::endl;
       }
       else
       {
           if (action == GLFW_PRESS)
           {
               std::cout << "key : " << key << ", " << char(key) << " : Shift Inactive, scancode: " << scancode << ", action: PRESS " << action << ", mods: " << mods << std::endl;
           }
           else if (action == GLFW_RELEASE)
           {
               std::cout << "key : " << key << ", " << char(key) << " : Shift Inactive, scancode: " << scancode << ", action: RELEASE " << action << ", mods: " << mods << std::endl;
           }
           else
           {
               std::cout << "key : " << key << ", " << char(key) << " : Shift Inactive, scancode: " << scancode << ", action: REPEAT " << action << ", mods: " << mods << std::endl;
           }
           
       }
   }

   // Pressing Arrow Keys

   if ((key == GLFW_KEY_UP && action == GLFW_PRESS) || (key == GLFW_KEY_UP && action == GLFW_REPEAT))
   {
       int xpos, ypos;
       glfwGetWindowPos(window, &xpos, &ypos);
       glfwSetWindowPos(window, xpos, ypos-5);
   }
   if ((key == GLFW_KEY_DOWN && action == GLFW_PRESS) || (key == GLFW_KEY_DOWN && action == GLFW_REPEAT))
   {
       int xpos, ypos;
       glfwGetWindowPos(window, &xpos, &ypos);
       glfwSetWindowPos(window, xpos, ypos + 5);
   }
   if ((key == GLFW_KEY_LEFT && action == GLFW_PRESS) || (key == GLFW_KEY_LEFT && action == GLFW_REPEAT))
   {
       int xpos, ypos;
       glfwGetWindowPos(window, &xpos, &ypos);
       glfwSetWindowPos(window, xpos - 5, ypos);
   }
   if ((key == GLFW_KEY_RIGHT && action == GLFW_PRESS) || (key == GLFW_KEY_RIGHT && action == GLFW_REPEAT))
   {
       int xpos, ypos;
       glfwGetWindowPos(window, &xpos, &ypos);
       glfwSetWindowPos(window, xpos + 5, ypos);
   }

   // Pressing +- Keys

   if ((key == GLFW_KEY_KP_ADD && action == GLFW_PRESS) || (key == GLFW_KEY_KP_ADD && action == GLFW_REPEAT))
   {
       int width, height;
       glfwGetWindowSize(window, &width, &height);
       glfwSetWindowSize(window, width+10, height+10);
   }
   if ((key == GLFW_KEY_KP_SUBTRACT && action == GLFW_PRESS) || (key == GLFW_KEY_KP_SUBTRACT && action == GLFW_REPEAT))
   {
       int width, height;
       glfwGetWindowSize(window, &width, &height);
       glfwSetWindowSize(window, width - 10, height - 10);
   }

   // Pressing F1 Key
   if ((key == GLFW_KEY_F1 && action == GLFW_PRESS))
   {
       int iconified = glfwGetWindowAttrib(window, GLFW_ICONIFIED);

       if (iconified)
       {
           glfwRestoreWindow(window);
       }
       else
       {
           glfwIconifyWindow(window);
       }
       
   }

   // Pressing Function Keys

   if ((key == GLFW_KEY_F5 && action == GLFW_PRESS))
   {
       GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_CROSSHAIR_CURSOR);
       glfwSetCursor(window, cursor);
   }
   if ((key == GLFW_KEY_F6 && action == GLFW_PRESS))
   {
       GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
       glfwSetCursor(window, cursor);
   }
   if ((key == GLFW_KEY_F7 && action == GLFW_PRESS))
   {
       GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_HAND_CURSOR);
       glfwSetCursor(window, cursor);
   }
   if ((key == GLFW_KEY_F8 && action == GLFW_PRESS))
   {
       GLFWcursor* cursor = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
       glfwSetCursor(window, cursor);
   }
}

//This function gets called when the mouse moves over the window.
void mouse_cursor(GLFWwindow* window, double x, double y)
{
   std::cout << "cursor pos: " << x << ", " << y << std::endl;
}

//This function gets called when a mouse button is pressed.
void mouse_button(GLFWwindow* window, int button, int action, int mods)
{
   std::cout << "button : "<< button << ", action: " << action << ", mods: " << mods << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (yoffset < 0)
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glfwSetWindowSize(window, width - 10, height - 10);
    }
    else
    {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        glfwSetWindowSize(window, width + 10, height + 10);
    }
}

void window_size_callback(GLFWwindow* window, int width, int height)
{
    std::cout << "WIDTH = " << width << " , HEIGHT = " << height << std::endl;
}

void window_focus_callback(GLFWwindow* window, int focused)
{
    if (focused)
    {
        /*std::cout << "Focussed";*/
    }
    else
    {
        /*std::cout << "Focus Lost";*/
        glfwFocusWindow(window);
    }
}

//Initialize OpenGL state. This function only gets called once.
void init()
{
    // hw1 code
    int x, y;

   std::ostringstream oss;
   //Get information about the OpenGL version supported by the graphics driver.	
   oss << "Vendor: "       << glGetString(GL_VENDOR)                    << std::endl;
   oss << "Renderer: "     << glGetString(GL_RENDERER)                  << std::endl;
   oss << "Version: "      << glGetString(GL_VERSION)                   << std::endl;
   oss << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION)  << std::endl;

   // hw1 code
   oss << "Window Width: " << window::size[0] << std::endl;
   oss << "Window Height: " << window::size[1] << std::endl;
   //oss << "Window Position: " << getWindo << std::endl;
   

   //Output info to console
   std::cout << oss.str();

   //Output info to file
   std::fstream info_file("info.txt", std::ios::out | std::ios::trunc);
   info_file << oss.str();
   info_file.close();


   //Set the color the screen will be cleared to when glClear is called
   glClearColor(window::clear_color[0], window::clear_color[1], window::clear_color[2], window::clear_color[3]);
}


// C++ programs start executing in the main() function.
int main(void)
{
    int xpos, ypos;
   GLFWwindow* window;

   
    

   // Initialize the library
   if (!glfwInit())
   {
      return -1;
   }

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
   glfwSetScrollCallback(window, scroll_callback);
   glfwSetWindowSizeCallback(window, window_size_callback);
   glfwSetWindowFocusCallback(window, window_focus_callback);

   // Make the window's context current
   glfwMakeContextCurrent(window);
   

   init();

   glfwGetWindowPos(window, &xpos, &ypos);

   std::cout << "Window Position : [" << xpos << "," << ypos << "]" << std::endl << std::endl;


   // Loop until the user closes the window 
   while (!glfwWindowShouldClose(window))
   {
      display(window);
      if (glfwGetTime() >= 5)
      {
          if (glfwGetTime() >= 10)
          {
              glfwSetTime(0);
          }
          else
          {
              glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
          }
          
      }
      else
      {
          glClearColor(window::clear_color[0], window::clear_color[1], window::clear_color[2], window::clear_color[3]);
      }

      // Poll for and process events 
      glfwPollEvents();
   }

   glfwTerminate();
   return 0;
}