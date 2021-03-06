#include <iostream>
#include <string>
#include <assert.h>
#include <assert.h>

using namespace std;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "../../utils/Shader.h"

const float PI = 3.14159;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void drawArray(const GLuint& VAO);

GLuint setupGeometry();

GLfloat* generateCircle(float radius, int nPoints, int totalSize);

const GLuint& setupVao();

void setupVbo();

const GLuint WIDTH = 800, HEIGHT = 600;

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Triangulo! - Gabriel Bittencourt", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	cout << "Renderer: " << glGetString(GL_RENDERER) << endl;
	cout << "OpenGL version supported " << glGetString(GL_VERSION) << endl;

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	GLuint VAO = setupGeometry();
	
	glUseProgram(shader.ID);
	
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		drawArray(VAO);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, GL_TRUE);
	}
}

GLuint setupGeometry() {

	setupVbo();
	
	return setupVao();
}

GLfloat* generateCircle(float radius, int nPoints, int totalSize) {


	GLfloat* vertices = new GLfloat[totalSize];

	vertices[0] = 0.0;
	vertices[1] = 0.0;
	vertices[2] = 0.0;

	float angle = 0.0;
	float slice = 2 * PI / (float) nPoints;

	for (int i = 3; i < totalSize; i += 3, angle += slice) {

		vertices[i] = radius * cos(angle);
		vertices[i + 1] = radius * sin(angle);
		vertices[i + 2] = 0.0;
	}
	
	cout << "Teste:" << vertices;

	return vertices;
}

void setupVbo() {

	float radius = 0.5;
	int nPoints = 30;
	int totalSize = (nPoints + 1) * 6;

	GLfloat* vertices = generateCircle(radius, nPoints, totalSize);

	GLuint VBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(totalSize), vertices, GL_STATIC_DRAW);
}

const GLuint& setupVao() {
	GLuint VAO;

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

void drawArray(const GLuint& VAO) {
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(10);
	glPointSize(10);

	glBindVertexArray(VAO);

	glDrawArrays(GL_LINE_LOOP, 0, 29);

	glBindVertexArray(0);
}