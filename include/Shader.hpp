#ifndef SHADER_HPP
#define SHADER_HPP
#include <glad/glad.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>


#include <iostream>
#include <string>
#include <fstream>


class Shader {

private:
	GLuint m_shader;
public:
	~Shader();
	void createSrcPath(std::string& vertSrc, std::string& fragSrc, const char* vertName, const char* fragName);
	void uniformMat4(glm::mat4& matrix, const char* name);
	void uniform2f(float width, float height,const char* uniformName);
	void uniform3f(glm::vec3& color, const char* name);
	GLuint& getShader();
	void use();
	//shader functions that can be placed in its own class
	bool setUpShaders(std::string& vertPath, std::string& fragPath);
	bool readFile(std::string path, std::string& destination);
	bool compileShader(char type, GLuint& shader, const char* src);
	bool linkShaderProgram(GLuint& program, GLuint& vert, GLuint& frag);

	
};


#endif