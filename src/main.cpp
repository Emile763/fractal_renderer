#include <fstream>
#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#if defined(WIN32) || defined(_WIN32)
#include <Windows.h>
#else
#include <unistd.h>
#endif

#include "GL/gl3w.h"
#include <GLFW/glfw3.h>




typedef struct _COORDS_ {
    GLfloat x;
    GLfloat y;
    GLfloat s;
    GLfloat t;
} CoordinatesSet;

typedef struct _RGBA_ {
    GLfloat Red;
    GLfloat Green;
    GLfloat Blue;
    GLfloat Alpha;
} RGBAValues;



void compileShader(GLuint shaderID, char* sourcePointer) {
    glShaderSource(shaderID, 1, &sourcePointer, NULL);
    glCompileShader(shaderID);

    GLint Result = GL_FALSE;
    int InfoLogLength = 1024;
    char shaderErrorMessage[1024] = { 0 };

    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &Result);

    glGetShaderInfoLog(shaderID, InfoLogLength, NULL, shaderErrorMessage);
    if (strlen(shaderErrorMessage) != 0)
        std::cout << shaderErrorMessage << "\n";
}
size_t fileToString(std::string path, char** data) {
    std::ifstream file{ path, std::ios::ate | std::ios::binary };

    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: \"" + path + "\"");
    }

    size_t file_size = static_cast<size_t>(file.tellg());
    char* buffer = (char*)malloc((file_size + 1) * sizeof(char));

    file.seekg(0);
    file.read(buffer, file_size);
    file.close();

    buffer[file_size] = 0;

    *data = buffer;

    return file_size;
}


float zoom = 1.;
int a = 1;
double r(5), g(0), b(0);
double x(0), y(0);
int iteration = 100;
bool update = false;
bool close = false;
float seconds = 0;
float power = 10.f;
int width;
int height;
int fractalType = 0;
double cx = 0.3;
double cy = 0.5;
void fractalUpdate(/*float& zoom, float& x, float& y, int &iteration,*/ GLuint& program, GLuint& texture)
{
    glUseProgram(program);
    GLint p = glGetUniformLocation(program, "pos");
    glUniform2d(p, x, y);
    GLint z = glGetUniformLocation(program, "zoom");
    glUniform1f(z, zoom);
    GLint i = glGetUniformLocation(program, "iteration");
    glUniform1i(i, iteration);
    GLint pow = glGetUniformLocation(program, "power");
    glUniform1f(pow, power);
    GLint c = glGetUniformLocation(program, "c");
    glUniform2d(c, cx, cy);
    GLint type = glGetUniformLocation(program, "fractalType");
    glUniform1i(type, fractalType);
    GLint rgb = glGetUniformLocation(program, "rgb");
    glUniform3i(rgb, GLint(r), GLint(g), GLint(b));
    glBindTexture(GL_TEXTURE_2D, texture);
    glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glDispatchCompute(int(width/16) + 1, int(height/16)+1, 1);
    glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    glBindImageTexture(0, 0, 0, GL_FALSE, 0, GL_WRITE_ONLY, GL_RGBA32F);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    update = false;
}
bool mLeft = false, mRight = false;
bool left(0), right(0), up(0), down(0), z(0), q(0), s(0), d(0), A(0), e(0), space(0);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            mLeft = true;
            
        }
        if (action == GLFW_RELEASE)
        {
            mLeft = false;
        }
    }if (button == GLFW_MOUSE_BUTTON_RIGHT)
    {
        if (action == GLFW_PRESS)
        {
            mRight = true;
            
        }
        if (action == GLFW_RELEASE)
        {
            mRight= false;
        }
    }
    
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{


    
    if (key == GLFW_KEY_ESCAPE)
    {
        close = true;

    }
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_UP)up = true;
        if (key == GLFW_KEY_DOWN) down = true;
        if (key == GLFW_KEY_S) s = true;
        if (key == GLFW_KEY_W) z = true;
        if (key == GLFW_KEY_A) q = true;
        if (key == GLFW_KEY_D) d = true;
        if (key == GLFW_KEY_LEFT)
        {

            iteration = int(iteration / 1.1);
            update = true;
        }
        if (key == GLFW_KEY_RIGHT)
        {

            iteration = int(iteration * 1.1);
            update = true;
        }if (key == GLFW_KEY_Q)
        {

            power -= 0.1f;
            update = true;
        }if (key == GLFW_KEY_E)
        {

            power += 0.1f;
            update = true;
        }
        if (key == GLFW_KEY_SPACE)
        {
            fractalType = !fractalType;
            update = true;

        }
        
     
    }if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_UP)up = false;
        if (key == GLFW_KEY_DOWN) down = false;
        if (key == GLFW_KEY_S) s = false;
        if (key == GLFW_KEY_W) z = false;
        if (key == GLFW_KEY_A) q = false;
        if (key == GLFW_KEY_D) d = false;
    }
    std::cout << "Zoom : " << zoom << "      Position : " << x << " : " << y << "     Nombre d'iteration : " << iteration << "    Power: " << power  <<std::endl;

}

int main(int argc, char** argv) {
    /* ----- Init window ----- */
    
    std::cout << "       ____                     ____       " << std::endl;
    std::cout << "      |    |                   |Zoom|      " << std::endl;
    std::cout << "      |  Z |                   |  ^ |      " << std::endl;
    std::cout << "      |Haut|                   |  | |     " << std::endl;
    std::cout << "      |____|                   |__+_|      " << std::endl;
    std::cout << " ____  ____  ____         ____  ____  ____ " << std::endl;
    std::cout << "|    ||    ||    |       |    ||Zoom||    |" << std::endl;
    std::cout << "|  Q ||  S ||  D |       | <- ||  | || -> |" << std::endl;
    std::cout << "|gauc|| bas||Droi|       |itr-||  v ||itr+|" << std::endl;
    std::cout << "|he__||____||te__|       |____||____||____|" << std::endl;
    
    

    GLFWwindow* window;
    if (!glfwInit()) {
        exit(-1);
    }

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    const GLFWvidmode* mode = glfwGetVideoMode(monitor);

    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    width = mode->width;
    height = mode->height;

    window = glfwCreateWindow(width, height, "Fractal Render", monitor, NULL);

    if (!window) {
        exit(-2);
    }
    glfwMakeContextCurrent(window);

    if (gl3wInit() != 0) {
        exit(-3);
    }
    glfwWindowHint(GLFW_SAMPLES, 4);

    glEnable(GL_MULTISAMPLE);

    /* ----- Quad Context ----- */

    RGBAValues* quadTexture = new RGBAValues[static_cast<size_t>(width) * static_cast<size_t>(height)];
    CoordinatesSet* quadVertex = new CoordinatesSet[4];
    unsigned short int* quadIndex = new unsigned short int[4];

    quadVertex[0].x = -1.f;
    quadVertex[0].y = 1.f;

    quadVertex[1].x = -1.0f;
    quadVertex[1].y = -1.0f;

    quadVertex[2].x = 1.0f;
    quadVertex[2].y = 1.0f;

    quadVertex[3].x = 1.0f;
    quadVertex[3].y = -1.0f;

    quadIndex[0] = 0;
    quadIndex[1] = 1;
    quadIndex[2] = 2;
    quadIndex[3] = 3;

    quadVertex[0].s = 0.0f;
    quadVertex[0].t = 0.0f;
    quadVertex[1].s = 0.0f;
    quadVertex[1].t = 1.0f;
    quadVertex[2].s = 1.0f;
    quadVertex[2].t = 0.0f;
    quadVertex[3].s = 1.0f;
    quadVertex[3].t = 1.0f;

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            quadTexture[x + y * width].Red = 1.0f;
            quadTexture[x + y * width].Green = .5f;
            quadTexture[x + y * width].Blue = .0f;
            quadTexture[x + y * width].Alpha = 1.0f;
        }
    }
    
    /* ----- Render Context ----- */

    GLuint quadIBO;
    GLuint quadVBO;
    GLuint quadVAO;
    GLuint quadTextureID;

    /* Cr�ation de la texture */
    glGenTextures(1, &quadTextureID);
    glBindTexture(GL_TEXTURE_2D, quadTextureID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenBuffers(1, &quadIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(unsigned short int), quadIndex, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glGenBuffers(1, &quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CoordinatesSet) * 4, 0, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(CoordinatesSet) * 4, quadVertex);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    glGenVertexArrays(1, &quadVAO);
    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, quadIBO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(sizeof(GLfloat) * 2));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    /* ----- Compute Shader ----- */


    GLuint computeShaderID;
    GLuint csProgramID;
    char* computeShader = 0;

    GLint Result = GL_FALSE;
    int InfoLogLength = 1024;
    char ProgramErrorMessage[1024] = { 0 };

    computeShaderID = glCreateShader(GL_COMPUTE_SHADER);

    fileToString("compute.shader", &computeShader);
    compileShader(computeShaderID, computeShader);

    csProgramID = glCreateProgram();

    glAttachShader(csProgramID, computeShaderID);
    glLinkProgram(csProgramID);
    glDeleteShader(computeShaderID);

    /* ----- Vertex shaders and Fragments shaders ----- */

    GLuint vertexShaderID;
    GLuint fragmentShaderID;
    GLuint programID;

    Result = GL_FALSE;
    InfoLogLength = 1024;
    memset(ProgramErrorMessage, 0, 1024);

    char* vertexShader = 0;
    char* fragmentShader = 0;

    vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    fileToString("vertex.shader", &vertexShader);
    fileToString("fragment.shader", &fragmentShader);

    compileShader(vertexShaderID, vertexShader);
    compileShader(fragmentShaderID, fragmentShader);

    programID = glCreateProgram();

    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);

    glLinkProgram(programID);

    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    glGetProgramiv(programID, GL_LINK_STATUS, &Result);
    glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    glGetProgramInfoLog(programID, InfoLogLength, NULL, &ProgramErrorMessage[0]);


    if (strlen(ProgramErrorMessage) != 0)
        std::cout << ProgramErrorMessage << "\n";
    


    /*Fonctions*/
    
        

    
    /*Déclaration des variables*/


    
        

    fractalUpdate(csProgramID, quadTextureID);
    

    /* ----- Render loop ----- */
    while (!close) {

        glfwPollEvents();
        
        glfwSetKeyCallback(window, key_callback);
        glfwSetMouseButtonCallback(window, mouse_button_callback);
        



#if defined(WIN32) || defined(_WIN32)
        Sleep(40);
#else
        usleep(40000);
#endif

        glEnable(GL_CULL_FACE);

        /* ----- Actual render ----- */
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (mLeft)
        {

            glfwGetCursorPos(window, &cx, &cy);
            cx = cx / width * 2. - 1.;
            cy = cy / height * 2. - 1.;
            std::cout << cx << " : " << cy << std::endl;
            update = true;
        }if (mRight)
        {

            glfwGetCursorPos(window, &r, &g);
            
            if (r < width / 3.)
            {
                r = int( 3.* r / width * 50);
                b = 0;
            }
            else if (r > 2. * width / 3.)
            {
                b = int(-3. * (r- 2. * width / 3.) / width * 50 + 50);
                r = 0;
                std::cout << b << std::endl;
            }
            else
            {
                b = int(3. * (r - width / 3.) / width * 50);
                r = -b + 50;
            }
            g = int(g / height * 50);
            
            update = true;
        }
        seconds = float(glfwGetTime());
        glfwSetTime(0);

        if (up)
        {

            zoom *=( 1.f + seconds);

            update = true;
        }
        if (down)
        {
            zoom /=( 1.f  + seconds);


            update = true;
        }
        
        if (s)
        {
            y += (double)(1.f / zoom) * seconds;
            update = true;

        }
        if (z)
        {
            y -= (double)(1.f / zoom) * float(seconds);
            update = true;

        }
        if (q)
        {
            x -= (double)(1.f / zoom) * float(seconds);
            update = true;
        }
        if (d)
        {
            x += (double)(1.f / zoom) * float(seconds);
            update = true;
        }
        //glUseProgram(programID);
        

        if (update)
        {
            fractalUpdate(csProgramID, quadTextureID);

        }
        
       
        
        glUseProgram(programID);

        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, quadVBO);

        glBindVertexArray(quadVAO);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, quadTextureID);

        glDrawElements(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_SHORT, (GLvoid*)0);



        glBindTexture(GL_TEXTURE_2D, 0);
        glActiveTexture(0);

        glBindVertexArray(0);

        glUseProgram(0);

        
        glfwSwapBuffers(window);
    }

    glfwDestroyWindow(window);
    glfwTerminate();


    return 0;
}















