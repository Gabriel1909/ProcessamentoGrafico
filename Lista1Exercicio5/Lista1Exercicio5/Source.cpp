#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "../../utils/Shader.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void drawArray(const GLuint& VAO);

GLuint setupGeometry();

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

void setupVbo() {

	GLfloat vertices[] = {
		-0.6,0.1,0.0,1.0,0.0,0.0,
		-0.3,-0.3,0.0,0.0,1.0,0.0,
		-0.9,-0.3,0.0,0.0,0.0,1.0,
		-0.6,-0.7,0.0,1.0,1.0,0.0,

		0.1,0.1,0.0,1.0,0.0,0.0,
		0.4,-0.3,0.0,0.0,1.0,0.0,
		-0.2,-0.3,0.0,0.0,0.0,1.0,
		0.1,-0.7,0.0,1.0,1.0,0.0,

		-0.25,0.7,0.0,1.0,0.0,0.0,
		0.05,0.3,0.0,0.0,1.0,0.0,
		-0.55,0.3,0.0,0.0,0.0,1.0,
		-0.25,-0.1,0.0,1.0,1.0,0.0,
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

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

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

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLES, 1, 3);

	glDrawArrays(GL_LINE_LOOP, 4, 3);
	glDrawArrays(GL_LINE_LOOP, 5, 3);

	glDrawArrays(GL_POINTS, 8, 3);
	glDrawArrays(GL_POINTS, 9, 3);

	glBindVertexArray(0);
}