#pragma once
#include <GLFW/glfw3.h>
#include <glad/glad.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void init_glfw();
void processInput(GLFWwindow* window);
float triangle_vertices();	//
int vao_vertex();
int  prep_vertex();