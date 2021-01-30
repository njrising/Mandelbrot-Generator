// Tool for creating mandelbrot fractals
// The purpose in creating this program is to render fractals real-time using
// the capabilities of the GPU and as an exercise in testing threading and other
// methods for high-performance rendering.
// Creating mandelbrot fractal uses a simple algorithm that produces complex images.
// These algorithms are well known and well documented and simple to understand
// which is part of the appeal of these fractals in general.
//
// Author: Nathan Rising
// Last Revision: 01/30/2021
//
// TO DO
// 1. Add support for inputting and outputting files
// 2. Add support for libpng for generating png files
// libpng is an opensource software for reading/writing png files
// libpng is compatible with C so support should be added.

#include "Opengl.h"
#include<chrono>

int main()
{
    // instantiate the openGL context
    OpenGL* program = OpenGL::getInstance();
    // obtain location of uniform variables
    int re = glGetUniformLocation(program->shader(),"re");
    int im = glGetUniformLocation(program->shader(),"im");
    int mItr = glGetUniformLocation(program->shader(),"mItr");
    // set uniform variable default maximum iterations to 200.
    glUniform1i(mItr,200);
    // Rendering loop/To exit the program, exit the window or press the space bar.
    while(!program->close()){

    program->poll();
    // Begin timer to test the speed of rendering to entire screen
    auto start = std::chrono::high_resolution_clock::now();
    // algorithm loop/ can this be placed inside shader?
    for(int i = 0;i < 1302;i++){
        for(int j = 0;j < 730;j++){
            glUniform1f(re,i);
            glUniform1f(im,j);
            program->render();
        }
    }
    // End timer and output to screen
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout<<"Elapsed time: "<<elapsed.count()<<"s\n";

    // Swap buffers
    program->post();

    // thinking of ways to regulate rate of rendering...
    Sleep(250);

    }
    program->delBuffer();
    glfwTerminate();
    return 0;
}

void increment(int& reference, const int by)
{
    reference += by;
}

void key_callback(GLFWwindow*window, int key, int scancode, int action, int mode){
    if((key == GLFW_KEY_ESCAPE||key == GLFW_KEY_SPACE) && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if((key == GLFW_KEY_UP && action == GLFW_PRESS))
    {
        OpenGL::getInstance()->incr_Iterations();
    }
}
