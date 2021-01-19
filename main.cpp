#include "Opengl.h"
#include<chrono>
#include<omp.h>
const int nthreads = 2;

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
    omp_set_num_threads(nthreads);
    //# pragma omp parallel
    //{
    //int id = omp_get_thread_num();
    //int nthrds= omp_get_num_threads();
    //# pragma omp parallel for
    for(int i = 0;i < 1302;i++){
        for(int j = 0;j < 730;j++){
            glUniform1f(re,i);
            glUniform1f(im,j);
            program->render();
        }
    }
   // } //thread

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
