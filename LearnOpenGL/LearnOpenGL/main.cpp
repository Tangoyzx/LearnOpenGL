// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

#include <vector>

// GLFW
#include <GLFW/glfw3.h>

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"
#include "Example1.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void DoMovement(Camera * camera, bool * keys);
GLuint GetCommonUniformBuffer();

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;

Camera *camera;

int screenWidth = 800;
int screenHeight = 600;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow * window = glfwCreateWindow(screenWidth, screenHeight, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	camera = new Camera(glm::radians(90.0f), (float)screenWidth / (float)screenHeight);

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// Options
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Failed to initialize GLEW" << std::endl;
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	GLuint uboId = GetCommonUniformBuffer();
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboId);
	
	auto example = new Example1();
	example->Init(camera, uboId);

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		DoMovement(camera, keys);

		example->OnTick();

		glfwSwapBuffers(window);
	}

	glfwTerminate();

	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (action == GLFW_PRESS)
		keys[key] = true;
	else if (action == GLFW_RELEASE)
		keys[key] = false;
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	float radian_x_offset = glm::radians(-xoffset * 0.1f);
	float radian_y_offset = glm::radians(yoffset * 0.1f);

	glm::vec3 new_rotation = camera->GetTransform()->GetRotation();
	new_rotation.x += radian_y_offset;
	new_rotation.y += radian_x_offset;
	camera->GetTransform()->SetRotation(new_rotation);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->isUse = (yoffset > 0);
}

void DoMovement(Camera * camera, bool * keys)
{
	glm::vec3 foward = camera->GetTransform()->forward();
	glm::vec3 right = camera->GetTransform()->right();
	glm::vec3 movement = glm::vec3(0, 0, 0);

	if (keys[GLFW_KEY_W]) {
		movement += foward;
	}

	if (keys[GLFW_KEY_S]) {
		movement -= foward;
	}

	if (keys[GLFW_KEY_A]) {
		movement -= right;
	}

	if (keys[GLFW_KEY_D]) {
		movement += right;
	}

	glm::vec3 orgPos = camera->GetTransform()->GetTranslation();
	camera->GetTransform()->SetTranslation(orgPos + movement * 0.1f);
}

GLuint GetCommonUniformBuffer()
{
	GLuint uboId;
	glGenBuffers(1, &uboId);
	glBindBuffer(GL_UNIFORM_BUFFER, uboId);
	glBufferData(GL_UNIFORM_BUFFER, 160, NULL, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	return uboId;
}