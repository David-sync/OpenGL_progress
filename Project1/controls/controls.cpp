#include "controls.hpp"


glm::mat4 ProjectionMatrix;
glm::mat4 ViewMatrix;

glm::vec3 position = glm::vec3(0, 0, 5);

float horizontalAngle = 3.14f; // mặc định camera khi được spawn thì sẽ nhìn vào hướng nào nhỉ, sao lại cần xoay 180 mới nhìn thấy object??

float verticalAngle = 0.0f;

float initialFoV = 45.0f;

float speed = 3.0f;
float mouseSpeed = 0.5f;

void computeMatricesFromInputs(GLFWwindow *window, float deltaTime)
{



	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// reset chuột về tâm màn hình
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	glfwSetCursorPos(window, width/2, height/2);


	horizontalAngle += mouseSpeed * deltaTime * float(width / 2 - xpos);
	verticalAngle += mouseSpeed * deltaTime * float(height / 2 - ypos);

	glm::vec3 direction(cos(verticalAngle) * sin(horizontalAngle), sin(verticalAngle), cos(verticalAngle) * cos(horizontalAngle));

	glm::vec3 right = glm::vec3(sin(horizontalAngle - 3.14f / 2.0f), 0, cos(horizontalAngle - 3.14f / 2.0f));

	glm::vec3 up = glm::cross(right, direction);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		position += direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		position -= direction * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
	{
		position += right * deltaTime * speed;
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
	{
		position -= right * deltaTime * speed;
	}

	ProjectionMatrix = glm::perspective(glm::radians(initialFoV), (float)width / (float)height, 0.1f, 100.0f);
	ViewMatrix = glm::lookAt(position, position + direction, up);

}



void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	initialFoV -= (float)yoffset * 5.0f;

	if (initialFoV < 5.0f)
	{
		initialFoV = 5.0f;
	}
	if (initialFoV > 120.0f)
	{
		initialFoV = 120.0f;
	}
}

glm::mat4 getProjectionMatrix()
{
	return ProjectionMatrix;
}
glm::mat4 getViewMatrix()
{
	return ViewMatrix;
}