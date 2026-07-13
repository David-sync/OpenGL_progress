#pragma once
#ifndef CONTROLS_H
#define CONTROLS_H

#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>



void computeMatricesFromInputs(GLFWwindow *window, float deltaTime);
glm::mat4 getProjectionMatrix();
glm::mat4 getViewMatrix();
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);

#endif