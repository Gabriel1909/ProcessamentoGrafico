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
		0.0, 0.6, 0.0,
		-0.6, -0.5, 0.0,
		0.6, -0.3, 0.0,
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
	glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(8);
	glPointSize(30);

	glBindVertexArray(VAO);

	glUniform4f(colorLoc, 0.3f, 0.46f, 0.73f, 1.0f);
	glDrawArrays(GL_LINE_LOOP, 0, 3);

	glUniform4f(colorLoc, 1.0f, 0.0f, 0.0f, 1.0f);
	glDrawArrays(GL_POINTS, 0, 1);

	glUniform4f(colorLoc, 0.43f, 0.69f, 0.25f, 1.0f);
	glDrawArrays(GL_POINTS, 1, 1);

	glUniform4f(colorLoc, 0.3f, 0.59f, 0.82f, 1.0f);
	glDrawArrays(GL_POINTS, 2, 1);

	glBindVertexArray(0);
}