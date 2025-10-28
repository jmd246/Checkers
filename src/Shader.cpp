#include <Shader.hpp>

Shader::~Shader() {
	glDeleteShader(m_shader);
}
void Shader::use() {
	glUseProgram(m_shader);
}
GLuint& Shader::getShader(){
	return m_shader;
}

void Shader::createSrcPath(std::string& vertSrc, std::string& fragSrc, const char* vertName, const char* fragName) {
	vertSrc = std::string(ASSET_PATH) + vertName, fragSrc = std::string(ASSET_PATH) + fragName;

}

bool Shader::readFile(std::string path, std::string& destination) {
	std::ifstream inputFile(path);
	if (!inputFile.is_open()) {
		std::cerr << "Failed to open file " << path;
		return false;
	}
	std::string line;
	while (std::getline(inputFile, line)) {
		destination.append(line + "\n");
	}
	inputFile.close();
	return true;
}

bool Shader::compileShader(char type, GLuint& shader, const char* src) {
	type = std::tolower(type);
	if (type == 'v') {
		shader = glCreateShader(GL_VERTEX_SHADER);
	}
	else if (type == 'f') {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else {
		return false;
	}
	GLint success;
	char infolog[512];
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shader, 512, NULL, infolog);
		std::cerr << infolog;
		return false;
	}
	return true;
}
bool Shader::linkShaderProgram(GLuint& program, GLuint& vert, GLuint& frag) {
	program = glCreateProgram();
	GLint success;
	char log[512];

	glAttachShader(program, vert);
	glAttachShader(program, frag);

	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, log);
		std::cerr << "Failed in ling program\n" << log;
		return false;
	}
	glDeleteShader(vert);
	glDeleteShader(frag);
	return true;
}
bool Shader::setUpShaders(std::string& vertPath, std::string& fragPath) {
	std::string vertSrc, fragSrc;
	GLuint vertShader, fragShader;
	//read files
	if (!readFile(vertPath, vertSrc)) {
		return false;
	}
	if (!readFile(fragPath, fragSrc)) {
		return false;
	}

	const char* vShaderCode = vertSrc.c_str();
	const char* fShaderCode = fragSrc.c_str();

	if (!compileShader('v', vertShader, vShaderCode)) {
		return false;
	}
	if (!compileShader('f', fragShader, fShaderCode)) {
		return false;
	}
	if (!linkShaderProgram(m_shader, vertShader, fragShader)) {
		return false;
	}
	return true;
}
void Shader::uniform3f(glm::vec3& val, const char* name) {
	glUniform3fv(glGetUniformLocation(m_shader, name), 1, glm::value_ptr(val));
}
void Shader::uniform2f(float a, float b, const char* name) {
	GLint location = glGetUniformLocation(m_shader, name);
	glUniform2f(location, a, b);
}

void Shader::uniformMat4(glm::mat4& matrix, const char* name) {
	glUniformMatrix4fv(glGetUniformLocation(m_shader, name), 1, GL_FALSE, glm::value_ptr(matrix));
}
