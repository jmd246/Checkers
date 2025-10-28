#ifndef BOARD_HPP
#define BOARD_HPP

#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <string>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <Tile.hpp>
#include <Shader.hpp>
#include <PieceRenderer.hpp>

class Board {
public:
	float left, right, bottom, top;

	Board(int width, int height);
	void setBoardSizeAndScale(int w, int h);

	~Board();
	
	int getWidth() const;
	int getHeight() const;
	
	void clear();
	void draw();

	PieceRenderer& getPieceRenderer() {
		return pieceRenderer;
	}

	void setPos(int x, int y, GamePiece piece);
	GamePiece getPiece(int x, int y);
	Tile getTile(int x, int y);

	bool setUpBoard(std::string& vertPath, std::string& fragPath);

	GLuint getVAO() {
		return m_vao;
	}

	glm::vec2 getTileDimenstions() {
		return glm::vec2(tileWidth, tileHeight);
	}

	glm::mat4& getProj() {
		return proj;
	}
	glm::mat4& getModel() {
		return model;
	}
	void initBuffers();
	bool isDarkTile(int x, int y);

private:

	int m_width, m_height;

	float tileWidth, tileHeight;

	glm::mat4 proj,model;

	GLuint m_vbo, m_instance_vbo,m_vao, m_ebo;

	
	Shader m_shader;
	std::vector<std::vector<Tile>> m_grid;
	std::vector<Tile> m_instances;

	PieceRenderer pieceRenderer;


};

#endif // !BOARD_HPP
