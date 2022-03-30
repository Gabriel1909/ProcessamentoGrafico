#include <iostream>
#include <string>
#include <assert.h>
#include <assert.h>

using namespace std;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "../../utils/Shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void drawArray(const GLuint& VAO, GLint colorLoc);

GLuint setupGeometry();

const GLuint& setupVao();

void setupVbo();

const GLuint WIDTH = 800, HEIGHT = 600;

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lista 1 - Exercicio 8", nullptr, nullptr);
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

void setupVbo() {

	GLfloat vertices[] = {
		0.35, 0.5, 0.0,
		0.2, 0.3, 0.0,
		0.5, 0.3, 0.0,
		0.5, -0.2, 0.0,
		0.2, -0.2, 0.0,

		0.25, 0.25, 0.0,
		0.35, 0.25, 0.0,
		0.35, 0.15, 0.0,
		0.25, 0.15, 0.0,
		0.25, 0.25, 0.0,

		0.3, 0.25, 0.0,
		0.3, 0.15, 0.0,

		0.35, 0.2, 0.0,
		0.25, 0.2, 0.0,

		0.3, 0.05, 0.0,
		0.4, 0.05, 0.0,
		0.4, -0.2, 0.0,
		0.3, -0.2, 0.0,
		0.3, 0.05, 0.0,

		0.36, -0.02, 0.0,

		0.0, -0.2, 0.0,
		0.7, -0.2, 0.0,
	};

	GLuint VBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(8);
	glPointSize(5);

	glBindVertexArray(VAO);

	glUniform4f(colorLoc, 0.75f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUniform4f(colorLoc, 1.0f, 1.0f, 0.0f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 5, 3);
	glDrawArrays(GL_TRIANGLES, 7, 3);

	glUniform4f(colorLoc, 0.38f, 0.145f, 0.137f, 1.0f);
	glDrawArrays(GL_TRIANGLES, 14, 3);
	glDrawArrays(GL_TRIANGLES, 16, 3);

	glUniform4f(colorLoc, 0.96f, 0.58f, 0.27f, 1.0f);
	glDrawArrays(GL_LINES, 20, 2);

	glUniform4f(colorLoc, 0.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 3);

	glDrawArrays(GL_LINE_LOOP, 1, 4);

	glDrawArrays(GL_LINE_LOOP, 5, 4);

	glDrawArrays(GL_LINES, 10, 2);
	glDrawArrays(GL_LINES, 12, 2);

	glDrawArrays(GL_LINE_LOOP, 14, 4);

	glDrawArrays(GL_POINTS, 19, 1);

	glBindVertexArray(0);
}