// Nossa classezinha que lê o arquivo de shader e o compila na OpenGL
// Exemplo retirado de https://learnopengl.com/#!Getting-started/Shaders

#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

//GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

using namespace std;

class Shader {

public:
	GLuint ID;
	// Constructor generates the shader on the fly
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {

		// 1. Retrieve the vertex/fragment source code from filePath
		std::string vertexCode;
		std::string fragmentCode;
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;
		// ensures ifstream objects can throw exceptions:
		vShaderFile.exceptions(std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::badbit);

		try {
			// Open files
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragmentPath);
			std::stringstream vShaderStream, fShaderStream;
			// Read file's buffer contents into streams
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();
			// close file handlers
			vShaderFile.close();
			fShaderFile.close();
			// Convert stream into string
			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		// 2. Compile shaders
		GLuint vertex, fragment;
		GLint success;
		GLchar infoLog[512];

		vertex = createShader(GL_VERTEX_SHADER, vertexCode.c_str(), &success, infoLog);

		fragment = createShader(GL_FRAGMENT_SHADER, fragmentCode.c_str(), &success, infoLog);

		// Shader Program
		this -> ID = glCreateProgram();
		glAttachShader(this -> ID, vertex);
		glAttachShader(this -> ID, fragment);
		glLinkProgram(this -> ID);
		// Print linking errors if any
		glGetProgramiv(this -> ID, GL_LINK_STATUS, &success);

		if (!success) {
			glGetProgramInfoLog(this->ID, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}

		// Delete the shaders as they're linked into our program now and no longer necessery
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	GLuint createShader(GLenum glShader, const GLchar* vShaderCode, GLint* success, GLchar infoLog[512]) {

		GLuint shader = glCreateShader(glShader);
		glShaderSource(shader, 1, &vShaderCode, NULL);
		glCompileShader(shader);
		// Print compile errors if any
		glGetShaderiv(shader, GL_COMPILE_STATUS, success);

		if (!success) {
			glGetShaderInfoLog(shader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}

		return shader;
	}

	// Uses the current shader
	void Use() {
		glUseProgram(this -> ID);
	}

	void setBool(const std::string& name, bool value) const {
		glUniform1i(glGetUniformLocation(this -> ID, name.c_str()), (int)value);
	}

	void setInt(const std::string& name, int value) const {
		glUniform1i(glGetUniformLocation(this -> ID, name.c_str()), value);
	}

	void setFloat(const std::string& name, float value) const {
		glUniform1f(glGetUniformLocation(this -> ID, name.c_str()), value);
	}

	void setVec3(const std::string& name, float v1, float v2, float v3) const {
		glUniform3f(glGetUniformLocation(this -> ID, name.c_str()), v1, v2, v3);
	}

	void setVec4(const std::string& name, float v1, float v2, float v3, float v4) const {
		glUniform4f(glGetUniformLocation(this -> ID, name.c_str()), v1, v2, v3,v4);
	}

	void setMat4(const std::string& name, float *v) const {
		glUniformMatrix4fv(glGetUniformLocation(this -> ID, name.c_str()), 1, GL_FALSE, v);
	}
};