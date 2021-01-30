#include "OpenGL.h"

OpenGL* OpenGL::instance = nullptr;
int OpenGL::mItr = 200;
//const int OpenGL::xpos = 1366;
//const int OpenGL::ypos = 766;
const int OpenGL::WIDTH = 1302;
const int OpenGL::HEIGHT = 730;

OpenGL::OpenGL(){
    // Initialize glfw
    glfwInit();
    // Set window hints. These lines should be commented out for other machines
    // If the version is not available, backward compatibility ensures the
    // highest version compatible with the computer is used.
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    // Some glfw settings
    glfwWindowHint(GLFW_RESIZABLE,GLFW_FALSE);
    glfwSetInputMode(window,GLFW_STICKY_KEYS,GL_TRUE);
    glfwWindowHint(GLFW_SAMPLES,4);
    glEnable(GL_MULTISAMPLE);
    // Create window and context
    window = glfwCreateWindow(WIDTH,HEIGHT,"A Most Excellent Window",0,0);
    if(!window){glfwTerminate();std::cout<<"Could not create window\n";}
    glfwMakeContextCurrent(window);
    // Initialize glew
    glewExperimental = GL_TRUE;
    if(glewInit()!=GLEW_OK){std::cout<<"Could not initialize GLEW\n";}

    glViewport(0,0,WIDTH,HEIGHT);

    // Output some useful information
    std::cout<<"Vendor: "<<glGetString(GL_VENDOR)<<'\n';
    std::cout<<"Renderer: "<<glGetString(GL_RENDERER)<<'\n';
    std::cout<<"Version: "<<glGetString(GL_VERSION)<<'\n';
    std::cout<<"Shading language version: "<<glGetString(GL_SHADING_LANGUAGE_VERSION)<<'\n';

    // Get the monitor's screen resolution
    const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    static const int xpos = mode->width;
    static const int ypos = mode->height;

    // Set window in the center of the screen
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glfwSetWindowPos(window,(xpos>>1)-(width>>1),(ypos>>1)-(height>>1));
    // Set the keyback function
    glfwSetKeyCallback(window,key_callback);
    // Compile vertex shader
    vshader();
    // Compile fragment shader
    fshader();
    // Link the shaders
    linkShaders();
    // Create vao object
    genBuffer();

    // Send information about the window size to the shader
    int loc = glGetUniformLocation(shaderProgram,"width");
    int loc1 = glGetUniformLocation(shaderProgram,"height");
    glUniform1f(loc,width);
    glUniform1f(loc1,height);
}

OpenGL* OpenGL::getInstance()
{
    if(!instance) {
        instance = new OpenGL();
    }
    return instance;
}

void OpenGL::poll(){
    glfwPollEvents();
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}
void OpenGL::post(){
    glfwSwapBuffers(window);
}
int OpenGL::close(){
    return glfwWindowShouldClose(window);
}
void OpenGL::vshader(){
    vertexshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexshader,1,&vertex_shader,0);
    glCompileShader(vertexshader);;
    glGetShaderiv(vertexshader, GL_COMPILE_STATUS,&s_Compile);
    if(!s_Compile){
        glGetShaderInfoLog(vertexshader,300,0,infolog);
        std::cout<<"Vertex shader compilation failed\n";
    }
}
void OpenGL::fshader(){
    fragmentshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentshader, 1,&fragment_shader,0);
    glCompileShader(fragmentshader);
    glGetShaderiv(fragmentshader, GL_COMPILE_STATUS, &s_Compile);
    if(!s_Compile){glGetShaderInfoLog(fragmentshader, 300, 0, infolog);
        std::cout<<"Fragment shader compilation failed"<<infolog<<'\n';
    }
}
void OpenGL::linkShaders(){
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram,  vertexshader);
    glAttachShader(shaderProgram, fragmentshader);
    glLinkProgram(shaderProgram);
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &s_Compile);
    if(!s_Compile){
        glGetProgramInfoLog(shaderProgram, 300, 0, infolog);
        std::cout<<"Linking shaders failed\n";
    }

    glUseProgram(shaderProgram);
    glDeleteShader(vertexshader);
    glDeleteShader(fragmentshader);
}
void OpenGL::genBuffer() {
    glGenVertexArrays(1,&VAO);
    glBindVertexArray(VAO);
}
void OpenGL::delBuffer(){
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glDeleteVertexArrays(1,&VAO);
}
void OpenGL::render(){
    glDrawArrays(GL_POINTS, 0, 1);
}
void OpenGL::incr_Iterations(){
    int loc = glGetUniformLocation(shaderProgram,"mItr");
    mItr+=100;std::cout<<mItr<<'\n';
    glUniform1i(loc,mItr);
}

OpenGL::~OpenGL()
{
    if(instance != NULL)
    {
        delete instance;
    }
}
//void OpenGL::test_callback(GLFWwindow*window, int key, int scancode, int action, int mode){
//    if((key == GLFW_KEY_ESCAPE||key == GLFW_KEY_SPACE) && action == GLFW_PRESS)
//    {
//        glfwSetWindowShouldClose(window, GL_TRUE);
//    }
//
//    if((key == GLFW_KEY_UP && action == GLFW_PRESS))
//    {
//        incr_Iterations();
//    }
//}

const char *OpenGL::fragment_shader = "\
#version 330 core\n\
in vec3 fragmentColor;\
out vec3 color;\
void main(){\
    color = fragmentColor;\
}\
";

const char *OpenGL::vertex_shader ="\
#version 330 core\n\
uniform float re;\
uniform float im;\
uniform int mItr;\
uniform float width;\
uniform float height;\
float xz = -.68;\
float yz = 0.37;\
float x_w = 0.05 * (width/height);\
float y_w = 0.05;\
\
int mandelbrot(float re, float im)\
{\
    int itr = 0;\
    float xx = 0.0,yy = 0.0,x = 0.0,y = 0.0;\
    while((x * x + y * y <= 4) && (itr < mItr)){\
        x = xx * xx - yy * yy + re * (x_w/width) + xz;\
        y = 2.0 * xx * yy + im * (y_w/height) + yz;\
        xx = x;\
        yy = y;\
        itr++;\
    }\
return itr;\
}\
vec3 colorPalette(int iter)\
{\
    if (iter == mItr){return vec3(0.0,0.471,0.953);}\
    else{return vec3(cos(iter*0.01227*1),cos(iter*0.01227*3),cos(iter*0.01227*5));}\
}\
out vec3 fragmentColor;\
\
void main()\
{\
    float px = re * (x_w/width) + xz;\
    float py = im * (y_w/height) + yz;\
    gl_Position = vec4((2.0/x_w) * px - (x_w + 2.0 * xz)/x_w, (2.0/y_w) * py - (y_w + 2.0 * yz)/y_w, 0.0, 1.0);\
    int iter = mandelbrot(re, im);\
    fragmentColor = colorPalette(iter);\
}\
";


void hello(){

}
//*fragmentColor = vec3(cos(iter*0.01227*1),cos(iter*0.01227*3),cos(iter*0.01227*5));\/*
