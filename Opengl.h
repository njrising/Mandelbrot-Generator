#ifndef OpenGL
#include<Gl/glew.h>
#include<GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <unistd.h>
#include <windows.h>

// Some function declarations
void key_callback(GLFWwindow*window, int key, int scancode, int action, int mode);

class OpenGL {
    private:
        OpenGL();                                    /* Initialize OpenGL libraries */
        static OpenGL* instance;
        static int mItr;
        static const int xpos;
        static const int ypos;
        static const int WIDTH;
        static const int HEIGHT;

        GLFWwindow* window;
        GLuint vertexshader;
        GLuint fragmentshader;
        GLchar infolog[300];
        GLuint shaderProgram;
        GLint s_Compile;
        GLuint VAO;
        static const GLchar *vertex_shader;
        static const GLchar *fragment_shader;
    public:
        static OpenGL* getInstance();
        void poll();                                 /* Poll clear screen */
        void post();                                 /* Swap buffers */
        int close();                                 /* Close window */
        void vshader();                              /* Compile vertex shader */
        void fshader();                              /* Compile fragment shader */
        void linkShaders();                          /* Link shaders */
        inline GLuint shader() const { return shaderProgram; }
        void genBuffer();                            /* Generate Buffers */
        void delBuffer();                            /* Delete Buffers*/
        void incr_Iterations();
        void render();
        ~OpenGL();
};
#endif

