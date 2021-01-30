// Tool for creating mandelbrot fractals
// The purpose in creating this program is to render fractals real-time using
// the capabilities of the GPU and as an exercise in testing threading and other
// methods for high-performance rendering.
// Creating mandelbrot uses a very simple algorithm that produces complex images.
// These algorithms are well known and well documented and simple to understand
// which is part of the appeal of fractals.
// Author: Nathan Rising
// Last Revision: 01/30/2021
// TO DO
// 1. Add support for inputting and outputting files
// 2. Add support for libpng for generating png files
// libpng is an opensource software for reading/writing png files
// libpng is compatible with C so support should be added.

#include "Opengl.h"
#include<chrono>

int main()
{
    OpenGL* program = OpenGL::getInstance();
    int re = glGetUniformLocation(program->shader(),"re");
    int im = glGetUniformLocation(program->shader(),"im");
    int mItr = glGetUniformLocation(program->shader(),"mItr");
    glUniform1i(mItr,200);
    while(!program->close()){
    program->poll();
    auto start = std::chrono::high_resolution_clock::now();

    for(int i = 0;i < 1302;i++){
        for(int j = 0;j < 730;j++){
            glUniform1f(re,i);
            glUniform1f(im,j);
            program->render();
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = finish - start;
    std::cout<<"Elapsed time: "<<elapsed.count()<<"s\n";
    program->post();
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
