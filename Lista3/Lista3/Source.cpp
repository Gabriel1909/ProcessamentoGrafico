#include <iostream>
#include <string>
#include <assert.h>
#include <assert.h>

using namespace std;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "../../utils/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

void drawArray(const GLuint& VAO, GLint colorLoc);

GLuint setupGeometry();

const GLuint& setupVao();

void setupVbo();

const GLuint WIDTH = 800, HEIGHT = 600;

using namespace glm;

float horizontal = 10.0;
float vertical = 10.0;

char tecla;

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lista 3", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	GLuint VAO = setupGeometry();
	
	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);

	glUseProgram(shader.ID);
	
	mat4 projection = ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	shader.setMat4("projection", value_ptr(projection));

	mat4 model = mat4(1);

	//model = rotate(model, (float)glfwGetTime(), vec3(0, 0, 1));
	//model = rotate(model, radians(teste), vec3(0, 0, 1));
	//model = scale(model, vec3(2.0, 2.0, 1));

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		switch (tecla) {
			case 'W':
				model = translate(model, vec3(0, 20, 0));
				break;

			case 'S':
				model = translate(model, vec3(0, -20, 0));
				break;

			case 'A':
				model = translate(model, vec3(-20, 0, 0));
				break;

			case 'D':
				model = translate(model, vec3(20, 0, 0));
				break;

			case 'Q':
				model = rotate(model, radians(10.0f), vec3(0, 0, 1));
				break;

			case 'E':
				model = rotate(model, radians(-10.0f), vec3(0, 0, 1));
				break;
		}

		tecla = NULL;

		shader.setMat4("model", value_ptr(model));

		drawArray(VAO, colorLoc);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &VAO);

	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {

			case GLFW_KEY_ESCAPE:
				glfwSetWindowShouldClose(window, GL_TRUE);
				break;

			case GLFW_KEY_W:
				tecla = 'W';
				break;

			case GLFW_KEY_S:
				tecla = 'S';
				break;

			case GLFW_KEY_A:
				tecla = 'A';
				break;

			case GLFW_KEY_D:
				tecla = 'D';
				break;

			case GLFW_KEY_Q:
				tecla = 'Q';
				break;

			case GLFW_KEY_E:
				tecla = 'E';
		}
	}
}

GLuint setupGeometry() {

	setupVbo();
	
	return setupVao();
}

void setupVbo() {

	GLfloat vertices[] = {
		0, 0, 0.0,
		100, 0, 0.0,
		50.0, 100, 0.0
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

	glUniform4f(colorLoc, 1.0f, 0.0f, 1.0f, 1.0f);

	glViewport(0, 0, WIDTH, HEIGHT);
	glDrawArrays(GL_TRIANGLES, 0, 3);


	glBindVertexArray(0);
}