#include <iostream>
#include <string>
#include <assert.h>
#include <assert.h>

using namespace std;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "../../utils/Shader.h"

const float PI = 3.14159;

char exercicio = '0';

int nPoints;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void drawArray(const GLuint& VAO, GLint colorLoc);

GLuint setupGeometry();

GLfloat* generateCircle(float radius, int totalSize);

const GLuint& setupVao();

void setupVbo();

const GLuint WIDTH = 800, HEIGHT = 600;

int main() {

	switch (exercicio) {

		case '0':
		case 'C':
		case 'D':
			nPoints = 50;
			break;

		case 'A':
			nPoints = 8;
			break;

		case 'B':
			nPoints = 5;
			break;
	}

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lista 1 Exercicio 6 - Gabriel Bittencourt", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	GLuint VAO = setupGeometry();
	
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);

	glUseProgram(shader.ID);
	
	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		drawArray(VAO, colorLoc);

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

GLfloat* generateCircle(float radius, int totalSize) {


	GLfloat* vertices = new GLfloat[totalSize];

	float angle = 0.0;
	float slice = 2 * PI / (GLfloat) nPoints;

	vertices[0] = 0.0;
	vertices[1] = 0.0;
	vertices[2] = 0.0;

	for (int i = 3; i < totalSize; i += 3, angle += slice) {

		vertices[i] = radius * cos(angle);
		vertices[i + 1] = radius * sin(angle);
		vertices[i + 2] = 0.0;
	}

	return vertices;
}

void setupVbo() {

	float radius = 0.5;
	int totalSize = (nPoints + 2) * 3;

	GLfloat* vertices = generateCircle(radius, totalSize);

	GLuint VBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, totalSize * sizeof(totalSize), vertices, GL_STATIC_DRAW);
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

void drawArray(const GLuint& VAO, GLint colorLoc) {

	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(10);
	glPointSize(10);

	glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
	glBindVertexArray(VAO);
	
	switch (exercicio) {

		case '0':
		case 'A':
		case 'B':
			glDrawArrays(GL_TRIANGLE_FAN, 0, nPoints + 2);
			break;

		case 'C':
			glDrawArrays(GL_TRIANGLE_FAN, 0, 45);
			break;

		case 'D':
			glDrawArrays(GL_TRIANGLE_FAN, 0, 10);
			break;
	}

	glBindVertexArray(0);
}