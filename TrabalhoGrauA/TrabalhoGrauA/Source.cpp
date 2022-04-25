#include <iostream>
#include <string>
#include <assert.h>
#include <assert.h>
#include <list>

using namespace std;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "../../utils/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

int main();


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLuint setupNave();
void atualizarNave(glm::mat4& model, Shader& shader, GLuint& VAO, const GLint& colorLoc);
void desenharNave(const GLuint& VAO, GLint colorLoc);

GLuint setupProjetil();
void atualizarProjetil(glm::mat4& model, Shader& shader, GLuint& VAO);
void desenharProjetil(const GLuint& VAO);

GLuint setupInimigos();
void atualizarInimigos(glm::mat4& model, Shader& shader, GLuint& VAO);
void desenharInimigos(const GLuint& VAO);

void validarColisao();

const GLuint WIDTH = 1920, HEIGHT = 1080;

const int posicaoInicialNave = 60;
int posicaoXNave = posicaoInicialNave;

const float velocidadeProjetil = 0.5;
float posicaoXProjetil = posicaoXNave;
float posicaoYProjetil = 70;

const float velocidadeInimigos = 0.05;
float posicaoInimigos = 0;
bool direcaoDireitaInimigos = true;

const int quantidadeInimigos = 4;
bool inimigos[quantidadeInimigos] = {true, true, true, true};

char tecla;

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Lista 3", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	int width, height;
	glfwGetFramebufferSize(window, &width, &height);

	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	GLuint vaoNave = setupNave();
	GLuint vaoProjetil = setupProjetil();
	GLuint vaoInimigos = setupInimigos();

	GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	assert(colorLoc > -1);

	glUseProgram(shader.ID);

	mat4 projection = ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	shader.setMat4("projection", value_ptr(projection));

	mat4 modelNave = mat4(1);
	mat4 modelProjetil = mat4(1);
	mat4 modelInimigos = mat4(1);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		atualizarNave(modelNave, shader, vaoNave, colorLoc);

		atualizarProjetil(modelProjetil, shader, vaoProjetil);
		
		validarColisao();

		atualizarInimigos(modelInimigos, shader, vaoInimigos);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vaoNave);

	glfwTerminate();
	return 0;
}

void atualizarNave(glm::mat4& model, Shader& shader, GLuint& VAO, const GLint& colorLoc) {
	
	switch (tecla) {
		case 'A':
			model = translate(model, vec3(-20, 0, 0));
			posicaoXNave -= 20;
			break;

		case 'D':
			model = translate(model, vec3(20, 0, 0));
			posicaoXNave += 20;
			break;
	}

	tecla = NULL;

	shader.setMat4("model", value_ptr(model));

	desenharNave(VAO, colorLoc);
}

void atualizarProjetil(glm::mat4& model, Shader& shader, GLuint& VAO) {

	model = translate(model, vec3(0, velocidadeProjetil, 0));
	posicaoYProjetil += velocidadeProjetil;

	if (posicaoYProjetil >= HEIGHT) {
		posicaoYProjetil = posicaoInicialNave;
		posicaoXProjetil = posicaoXNave;
		model = mat4(1);
		model = translate(model, vec3(posicaoXProjetil - posicaoInicialNave, 0, 0));
	}

	shader.setMat4("model", value_ptr(model));

	desenharProjetil(VAO);
}

void atualizarInimigos(glm::mat4& model, Shader& shader, GLuint& VAO) {
	if (posicaoInimigos >= 500) {
		direcaoDireitaInimigos = false;
	}

	else if (posicaoInimigos <= 0) {
		direcaoDireitaInimigos = true;
	}

	if (direcaoDireitaInimigos) {
		model = translate(model, vec3(velocidadeInimigos, 0, 0));
		posicaoInimigos += velocidadeInimigos;
	}

	else {
		model = translate(model, vec3(-velocidadeInimigos, 0, 0));
		posicaoInimigos -= velocidadeInimigos;
	}

	shader.setMat4("model", value_ptr(model));
	desenharInimigos(VAO);
}

GLuint setupNave() {

	//VBO
	GLfloat vertices[] = {
		50, 50, 0.0,
		70, 50, 0.0,
		posicaoXNave, 70, 0.0
	};

	GLuint VBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);


	//VAO
	GLuint VAO;

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

GLuint setupProjetil() {

	//VBO
	GLfloat vertices[] = {
		posicaoXNave, posicaoYProjetil, 0.0
	};

	GLuint VBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//VAO
	GLuint VAO;

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

GLuint setupInimigos() {

	//VBO
	GLfloat vertices[] = {
		50, 500, 0.0,
		70, 500, 0.0,
		60, 480, 0.0,

		80, 500, 0.0,
		100, 500, 0.0,
		90, 480, 0.0,

		110, 500, 0.0,
		130, 500, 0.0,
		120, 480, 0.0,

		140, 500, 0.0,
		160, 500, 0.0,
		150, 480, 0.0
	};

	GLuint VBO;

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//VAO
	GLuint VAO;

	glGenVertexArrays(1, &VAO);

	glBindVertexArray(VAO);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}


void validarColisao() {

	if (posicaoYProjetil >= 480 && posicaoYProjetil <= 500) {

		for (int inimigo = 0, posicao = 50; inimigo < quantidadeInimigos; inimigo++, posicao += 30) {

			if (posicaoXProjetil >= posicao + posicaoInimigos && posicaoXProjetil <= posicao + 20 + posicaoInimigos) {
				inimigos[inimigo] = false;
			}
		}
	}
}


void desenharNave(const GLuint& VAO, GLint colorLoc) {
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(8);
	glPointSize(5);

	glBindVertexArray(VAO);

	glUniform4f(colorLoc, 1.0f, 0.0f, 1.0f, 1.0f);

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glBindVertexArray(0);
}

void desenharProjetil(const GLuint& VAO) {
	
	glBindVertexArray(VAO);

	glDrawArrays(GL_POINTS, 0, 1);

	glBindVertexArray(0);
}

void desenharInimigos(const GLuint& VAO) {

	glBindVertexArray(VAO);

	for (int i = 0; i < quantidadeInimigos; i++) {
		if (inimigos[i]) {
			glDrawArrays(GL_TRIANGLES, i * 3, 3);
		}
	}

	glBindVertexArray(0);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (action == GLFW_PRESS || action == GLFW_REPEAT) {
		switch (key) {

		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;

		case GLFW_KEY_B:
			tecla = 'B';
			break;

		case GLFW_KEY_A:
			tecla = 'A';
			break;

		case GLFW_KEY_D:
			tecla = 'D';
			break;
		}
	}
}