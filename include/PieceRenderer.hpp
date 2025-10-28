#ifndef PIECE_RENDERER
#define PIECE_RENDERER
#define _USE_MATH_DEFINES
#include <stdint.h>

#include <GLAD/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Shader.hpp>
#include <string>

class PieceRenderer{


public:
	
	glm::vec3 lightColor, darkColor;

	const float pi;

	PieceRenderer();
    // control how well piece is rendered
	uint32_t vertexCount = 0;

	//buffer objects
	GLuint vbo, vao;
	Shader m_shader;



	//set up the objects
	void init(int segments);
	
	void draw(const glm::vec2& pos, const glm::vec2& cellScale ,glm::vec3 col,float radius,glm::mat4& proj,  glm::mat4 model);
private: 
	std::string pieceVertPath, pieceFragPath;
};

#endif PIECE_RENDERER
