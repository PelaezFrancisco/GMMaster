#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Application.h"
#include "Display.h"


using namespace std;

int dis = 1;

float vert[] = {
        -0.5f, -0.5f, 0.0f, // left  
         0.5f, -0.5f, 0.0f, // right 
         0.0f,  0.5f, 0.0f  // top   
};
float vertices[] = {
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
    -0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
};

/*
* VERTEX INITIATION
* return.- void with vertex and fragment shader
*/
Display vertex_init() {
    //First Shader
    Display aux;
    aux.vertexShaderSource = "#version 330 core\n"
        "layout (location = 0) in vec3 aPos;\n"
        "void main()\n"
        "{\n"
        "gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
        "}\0";
    aux.fragmentShaderSource = "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main()\n"
        "{\n"
        "FragColor = vec4(1.0f,1.0f,0.2f,1.0f);\n"
        "}\0";
    return aux;
}
/*
* VERTEX PREPARATION
* returns.- shaderprogram() GL_PROGRAM
*/
int  prep_vertex() {

    //Brings vexrtex and shader from Display
    Display d1 = vertex_init();

    int vertexShader = vertex_program(d1);  //Vertex Shader Program
    
    int fragmentShader = fragment_program(d1); //Fragment shader program

    int shaderProgram = link_program(vertexShader, fragmentShader); //Linking Program

    return shaderProgram;
}

int vao_vertex() {

    unsigned int VBO, VAO, EBO;      //CREAMOS OBJETO PARA DIBUJAR
                                //VAO= Vertex Array Object
                                //VBO= Vertex Buffer Object
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    // 0. copy our vertices array in a buffer for OpenGL to use
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                 //Bind buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);  //Set buffer as Static_Draw
    //
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 1. then set the vertex attributes pointers
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   //Pointer to the position of vertex (location=0)
    glEnableVertexAttribArray(0);
    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    // 2. use our shader program when we want to render an object
    //glUseProgram(shaderProgram);  --> see render loop
    return VAO;
}

/*
* VERTEX SHADER PROGRAM
* gets.- Display.vertexShaderSource
* returns.- vertexShader GL_VERTEX_SHADER
*/
int vertex_program(Display d) {
    //BUILD AND COMPILE VERTEX PROGRAM
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);            //create vertex
    glShaderSource(vertexShader, 1, &d.vertexShaderSource, NULL); //attach vertex source
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
    return vertexShader;
}
/*
* FRAGMENT SHADER PROGRAM
* gets.- Display.fragmentShaderSource
* returns.- vertexShader GL_VERTEX_SHADER
*/
int fragment_program(Display d) {
    //BUILD AND COMPILE FRAGMENT SHADER PROGRAM
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);        //create fragment
    glShaderSource(fragmentShader, 1, &d.fragmentShaderSource, NULL); //attach fragment
    glCompileShader(fragmentShader);                                //compile fragment
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    return fragmentShader;
}
/*
* LINKING SHADER PROGRAM
* gets.- vertexShader, fragment shader
* returns.- shaderProgram glCreateProgram();
*/
int link_program(int vertexShader, int fragmentShader) {
    //BUILD LINK SHADERS
   // link shaders
    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);    //attach vertex
    glAttachShader(shaderProgram, fragmentShader);  //attach fragment
    glLinkProgram(shaderProgram);                   //link both
    // check for linking errors
    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);                   //delete vertex
    glDeleteShader(fragmentShader);                 //delete fragment

    return shaderProgram;
}