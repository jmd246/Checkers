#include <Board.hpp>
float vertices[] = {
	// positions      // colors
	-0.5f, -0.5f,     1.0f, 0.0f, 0.0f, // bottom left
	 0.5f, -0.5f,     0.0f, 1.0f, 0.0f, // bottom right
	 0.5f,  0.5f,     0.0f, 0.0f, 1.0f, // top right
	-0.5f,  0.5f,     1.0f, 1.0f, 1.0f  // top left
};


unsigned int indices[] = {
	0, 1, 2,
	2, 3, 0
};


Board::Board(int w, int h) : m_width(w), m_height(h), m_grid(h, std::vector<Tile>(w,Tile())) {
}

void Board::clear() {
	for (auto& row : m_grid) {
		for (auto& tile : row) {
			tile = Tile();
		}
	}
}

Board::~Board() {
	if (m_vao) {
		glDeleteVertexArrays(1, &m_vao);
	}
	if (m_vbo) {
		glDeleteBuffers(1,&m_vbo);
	}
	if (m_instance_vbo) {
		glDeleteBuffers(1, &m_instance_vbo);
	}
	if (m_ebo) {
		glDeleteBuffers(1,&m_ebo);
	}
	if (m_shader.getShader()) {
		glDeleteProgram(m_shader.getShader());
	}
}

void Board::draw() {
	m_shader.use();
	glBindVertexArray(m_vao);
	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, m_instances.size());
	glBindVertexArray(0);
	glUseProgram(0);

	pieceRenderer.init(128);


}


void Board::initBuffers() {
	

	glGenVertexArrays(1, &m_vao);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_instance_vbo);
	glGenBuffers(1, &m_ebo);

	//bind vert array
	glBindVertexArray(m_vao);

	//vert buffer
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//element buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);



	// position attribute (2 floats)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// color attribute (3 floats)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//-- instance data
	glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_instances.size() * sizeof(Tile), m_instances.data(), GL_STATIC_DRAW);


	// layout (location = 2) vec2 offset
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Tile), (void*)0);
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1); // one per instance

	// layout (location = 3) vec3 color
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Tile), (void*)(sizeof(glm::vec2)));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, 1); // one per instance
	glBindVertexArray(0); // unbind
	
}
bool Board::setUpBoard(std::string& vertPath, std::string& fragPath) {
	m_shader.createSrcPath(vertPath, fragPath, "checkerBoard.vert", "checkerBoard.frag");

	if (!m_shader.setUpShaders(vertPath, fragPath))
		return false;
	m_instances.clear();
	m_instances.reserve(m_width * m_height);

	tileWidth = 2.0f / m_width;
	tileHeight = 2.0f / m_height;

 	for (int y = 0; y < m_height; y++) {
		for (int x = 0; x < m_width; x++) {
			Tile& tile = m_grid[y][x];
			glm::vec2 pos = glm::vec2(-1.0f + tileWidth* ( x + 0.5f) ,
				-1.0f + tileHeight * (y + 0.5f));
			tile.setPos(pos);
			bool dark = isDarkTile(x, y);
			glm::vec3 col = dark ? glm::vec3(0.1f, 0.1f, 0.1f) : glm::vec3(0.9f, 0.9f, 0.9f);

			if (dark && (y != 3 && y != 4)) {
				GamePiece piece(pos, y > m_height/2 ? GamePiece::dark : GamePiece::light,  GamePiece::pawn);
				tile.setPiece(piece);
			}


			tile.setColor(col);
			m_instances.push_back(tile);
		}
	}
	


	return true;
}
void Board::setBoardSizeAndScale(int width, int height) {
	m_shader.use();
	float aspect = (float) width / (float)height;

	// Compute projection extents exactly the same way we build 'proj' so
	// tile positions are computed in the same world coordinate space.
	if (aspect > 1.0f) {
		left = -aspect; right = aspect;
		bottom = -1.0f; top = 1.0f;
	}
	else {
		left = -1.0f; right = 1.0f;
		bottom = -1.0f / aspect; top = 1.0f / aspect;
	}


	// Keep squares perfectly square regardless of window shape
	proj = glm::ortho(left, right, bottom, top);


	model = glm::mat4(1.0f);

	// Uniform scaling to keep everything centered and add some margin
	float scale = 0.6f; // smaller = more margin
	model = glm::scale(model, glm::vec3(scale, scale, 1.0f));

	// Recompute tile positions in the same world-space used by proj so the
	// top row / right column align exactly with the projection edges.
	if (!m_instances.empty()) {
		
        tileWidth = (right- left) / static_cast<float>(m_width);
		tileHeight = (top- bottom) / static_cast<float>(m_height);
		for (int y = 0; y < m_height; ++y) {
			for (int x = 0; x < m_width; ++x) {
				size_t idx = static_cast<size_t>(y) * static_cast<size_t>(m_width) + static_cast<size_t>(x);
				if (idx < m_instances.size()) {
					glm::vec2 pos = glm::vec2(
						left + tileWidth * (static_cast<float>(x) + 0.5f),
						bottom + tileHeight * (static_cast<float>(y) + 0.5f));
					m_instances[idx].setPos(pos);
				}
			}
		}
		
		// Push updated instance positions to GPU. Use glBufferSubData if buffer already created.
		if (m_instance_vbo) {
			glBindBuffer(GL_ARRAY_BUFFER, m_instance_vbo);
			glBufferSubData(GL_ARRAY_BUFFER, 0, m_instances.size() * sizeof(Tile), m_instances.data());
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			m_shader.uniform2f(tileWidth, tileHeight, "tileSize");
		}
	}

	m_shader.uniformMat4(proj, "proj");
	m_shader.uniformMat4(model, "model");

	initBuffers();

}

bool Board::isDarkTile(int x,int y) {
	return (x + y) % 2 == 0;
}

void Board::setPos(int x, int y, GamePiece piece) {
	if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
		Tile& tile = m_grid[y][x];
	 tile.getPiece().setCoat(piece.getCoat());
		tile.getPiece().setRank(piece.getRank());
	}
}
Tile Board::getTile(int x, int y) {
	if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
		return m_grid[y][x];
	}
	throw std::invalid_argument("Invalid position passed into function");
}
GamePiece Board::getPiece(int x, int y) {
	if (x >= 0 && x < m_width && y >= 0 && y < m_height) {
	  return m_grid[y][x].getPiece();
	}
	throw std::invalid_argument("Invalid position passed into function");
}
int Board::getHeight() const{
	return m_height;
}
int Board::getWidth() const{
	return m_width;
   
}