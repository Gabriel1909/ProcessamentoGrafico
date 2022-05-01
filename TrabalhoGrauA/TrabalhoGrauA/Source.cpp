#include <iostream>
#include <string>
#include <assert.h>
#include <assert.h>
#include <list>

using namespace std;

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include "stb_image.h"

#include "../../utils/Shader.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;

int main();

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

GLuint setupNave();
void atualizarNave(glm::mat4& model, Shader& shader, GLuint& VAO);
void desenharNave(const GLuint& VAO);

GLuint setupProjetil();
void atualizarProjetil(glm::mat4& model, Shader& shader, GLuint& VAO);
void desenharProjetil(const GLuint& VAO);

GLuint setupInimigos();
void atualizarInimigos(glm::mat4& model, Shader& shader, GLuint& VAO);
void desenharInimigos(const GLuint& VAO);
GLuint loadTexture(string path);

void validarColisao();

const GLuint WIDTH = 1920, HEIGHT = 1080;

const int posicaoInicialNave = 50;
int posicaoXNave = posicaoInicialNave;

const float velocidadeProjetil = 0.5;
float posicaoXProjetil = posicaoXNave;
float posicaoYProjetil = 60;

const float velocidadeInimigos = 0.05;
float posicaoInimigos = 0;
bool direcaoDireitaInimigos = true;

const int quantidadeInimigos = 4;
bool inimigos[quantidadeInimigos] = {true, true, true, true};

char tecla;

int main() {

	glfwInit();

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Trabalho Grau A", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	glfwSetKeyCallback(window, key_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

	Shader shader("../shaders/hello.vs", "../shaders/hello.fs");

	GLuint vaoNave = setupNave();
	GLuint vaoProjetil = setupProjetil();
	GLuint vaoInimigos = setupInimigos();

	//GLint colorLoc = glGetUniformLocation(shader.ID, "inputColor");
	//assert(colorLoc > -1);

	glUseProgram(shader.ID);

	glUniform1i(glGetUniformLocation(shader.ID, "tex_buffer"), 0);

	mat4 projection = ortho(0.0, 1920.0, 0.0, 1080.0, -1.0, 1.0);
	shader.setMat4("projection", value_ptr(projection));
	

	mat4 modelProjetil = mat4(1);
	mat4 modelInimigos = mat4(1);

	while (!glfwWindowShouldClose(window)) {

		glfwPollEvents();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);

		mat4 modelNave = mat4(1);

		atualizarNave(modelNave, shader, vaoNave);

		atualizarProjetil(modelProjetil, shader, vaoProjetil);
		
		validarColisao();

		atualizarInimigos(modelInimigos, shader, vaoInimigos);

		glfwSwapBuffers(window);
	}

	glDeleteVertexArrays(1, &vaoNave);

	glfwTerminate();
	return 0;
}

void atualizarNave(glm::mat4& model, Shader& shader, GLuint& VAO) {
	

	model = glm::translate(model, glm::vec3(59.5 + posicaoXNave, 159.5, 0));
	model = glm::scale(model, glm::vec3(50.0, 50.0, 1.0));
	model = glm::rotate(model, /*(float)glfwGetTime()*/ glm::radians(180.0f), glm::vec3(0, 0, 1));

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

	desenharNave(VAO);
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

	GLuint VBO, VAO, EBO;

	//VBO
	GLfloat vertices[] = {
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0, // superior direito
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // inferior direito
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // inferior esquerdo
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0  // superior esquerdo
	};

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//EBO
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};

	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//VAO
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	glEnableVertexAttribArray(2);

	loadTexture("../textures/nave.png");

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	return VAO;
}

GLuint setupProjetil() {

	//VBO
	GLfloat vertices[] = {
		posicaoXNave + 5, posicaoYProjetil, 0.0
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


void desenharNave(const GLuint& VAO) {
	
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glLineWidth(8);
	glPointSize(5);

	glBindBuffer(GL_ARRAY_BUFFER, 1);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 1);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawArrays(GL_POINTS, 0, 6);

	glBindTexture(GL_TEXTURE_2D, 0);
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

GLuint loadTexture(string path) {

	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_PROXY_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_PROXY_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_PROXY_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_PROXY_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int width, height, nrChannels;

	unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);

	if (data) {

		GLint rgbType = (nrChannels == 3) ? GL_RGB : GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, rgbType, width, height, 0, rgbType, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
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