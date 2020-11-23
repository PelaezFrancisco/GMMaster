#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>

#include "Application.h"
#include "Display.h"


using namespace std;

int dis = 1;

//Using glDrawElement
//Also change glbindbuffer
/*
* float vertices[] = {
    //X, Y, Z
     0.5f,  0.5f, 0.0f,  // top right
     0.5f, -0.5f, 0.0f,  // bottom right
   - 0.5f, -0.5f, 0.0f,  // bottom left
    -0.5f,  0.5f, 0.0f   // top left 
};
*/
float firstTriangle[] = {
       -0.9f, -0.5f, 0.0f,  // left 
       -0.0f, -0.5f, 0.0f,  // right
       -0.45f, 0.5f, 0.0f,  // top 
};
float secondTriangle[] = {
    0.0f, -0.5f, 0.0f,  // left
    0.9f, -0.5f, 0.0f,  // right
    0.45f, 0.5f, 0.0f   // top 
};


unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,  // first triangle
    1, 2, 3    // second triangle
};

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"gl_Position = vec4(aPos.x,aPos.y,aPos.z,1.0f);\n"
"}\0";

const char* fragmentShaderSource[2] = {
    "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f,1.0f,0.2f,1.0f);\n"
"}\0" ,

"#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"FragColor = vec4(1.0f,0.5f,0.2f,1.0f);\n"
"}\0"

};

/*
* VERTEX INITIATION
* return.- void with vertex and fragment shader
*/

/*
* VERTEX PREPARATION
* returns.- shaderprogram() GL_PROGRAM
*/
int  prep_vertex(int frag_i) {

    //Brings vexrtex and shader from Display

    int vertexShader = vertex_program(vertexShaderSource);  //Vertex Shader Program
    const char* frag_aux = fragmentShaderSource[frag_i];
    int fragmentShader = fragment_program(frag_aux); //Fragment shader program


    int shaderProgram = link_program(vertexShader, fragmentShader); //Linking Program

    return shaderProgram;
}

int vao_vertex(int vaoi) {

    unsigned int VBO[2], VAO[2], EBO;      //CREAMOS OBJETO PARA DIBUJAR
                                //VAO= Vertex Array Object
                                //VBO= Vertex Buffer Object
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    //glGenBuffers(1, &EBO);

    //FIRST TRIANGLE
    glBindVertexArray(VAO[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);                                 //Bind buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);  //Set buffer as Static_Draw
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   //Pointer to the position of vertex (location=0)
    glEnableVertexAttribArray(0);
    
    //SECOND TRIANGLE
    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);                                 //Bind buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);  //Set buffer as Static_Draw
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);   //Pointer to the position of vertex (location=0)
    glEnableVertexAttribArray(0);

    //glBindBuffer(GL_ARRAY_BUFFER, 0);
    //glBindVertexArray(0);
    // 2. use our shader program when we want to render an object
    //glUseProgram(shaderProgram);  --> see render loop
    return VAO[vaoi];
}

/*
* VERTEX SHADER PROGRAM
* gets.- Display.vertexShaderSource
* returns.- vertexShader GL_VERTEX_SHADER
*/
int vertex_program(const char* vertexShaderSource) {
    //BUILD AND COMPILE VERTEX PROGRAM
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
    return vertexShader;
}
/*
* FRAGMENT SHADER PROGRAM
* gets.- Display.fragmentShaderSource
* returns.- vertexShader GL_VERTEX_SHADER
*/
int fragment_program(const char* fragmentShaderSource) {
    //BUILD AND COMPILE FRAGMENT SHADER PROGRAM
    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);        //create fragment
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL); //attach fragment
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