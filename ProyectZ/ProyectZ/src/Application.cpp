#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "functions.h"

using namespace std;
//SHADER
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
    "}\0";
const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
    "}\0";

int main()
{   
    init_glfw();

    GLFWwindow* window = glfwCreateWindow(800, 600, "ProyectZ", NULL, NULL);

    //GLFW windoe creation
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
   // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
    

    //float* vert = triangle_vertices();
    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vert[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
    };

    //BUILD AND COMPILE VERTEX PROGRAM
    //vertex shader
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);            //create vertex
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL); //attach vertex source
    glCompileShader(vertexShader);                              //compile vertex
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //BUILD AND COMPILE FRAGMENT SHADER PROGRAM
    // fragment shader
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);        //create fragment
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //attach fragment
    glCompileShader(fragmentShader);                                //compile fragment
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    //BUILD LINK SHADERS
    // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);    //attach vertex
    glAttachShader(shaderProgram, fragmentShader);  //attach fragment
    glLinkProgram(shaderProgram);                   //link both
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);                   //delete vertex
    glDeleteShader(fragmentShader);                 //delete fragment


    unsigned int VBO, VAO;      //CREAMOS OBJETO PARA DIBUJAR
                                //VAO= Vertex Array Object
                                //VBO= Vertex Buffer Object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);
   
    // 0. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                 //Bind buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);  //Set buffer as Static_Draw
    // 1. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   //Pointer to the position of vertex (location=0)
    glEnableVertexAttribArray(0);
    // 2. use our shader program when we want to render an object
    //glUseProgram(shaderProgram);  --> see render loop

    //RENDER LOOP
    while (!glfwWindowShouldClose(window))
    {
        //input
        processInput(window);

        //rendering comand here...
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // glBindVertexArray(0); // no need to unbind it every time 

        //check and call events to swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}




