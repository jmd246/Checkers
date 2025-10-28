#include <PieceRenderer.hpp>

PieceRenderer::PieceRenderer() : lightColor(1.0f,0.0f,0.0f),darkColor(0.0f,0.0f,1.0f), pi(M_PI) {
}


void PieceRenderer::init(int segments) {
	// compile and link shader program for game pieces
	
	m_shader.createSrcPath(pieceVertPath, pieceFragPath, "piece.vert", "piece.frag");
	
	if (!m_shader.setUpShaders(pieceVertPath, pieceFragPath)) {
		std::cout << "ERROR IN SETTING UP SHADERS";
	}


	std::vector<glm::vec2> vertices;
	//push the center onto array
	
	vertices.emplace_back(glm::vec2(0, 0));
	//loop through vertex count
	for (int i = 0; i <= segments; i++) {
		float angle = 2.0f * pi * i / segments;
		vertices.emplace_back(glm::vec2(cos(angle), sin(angle)));
	}
	vertexCount = (int)vertices.size();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(glm::vec2), vertices.data(), GL_STATIC_DRAW);

	//vertex attribs

	glEnableVertexAttribArray(0);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*) 0 );


	glBindVertexArray(0);


}

void PieceRenderer::draw(const glm::vec2& pos,const glm::vec2& cellScale,glm::vec3 col, float radius,glm::mat4& proj,glm::mat4 model) {
	m_shader.use();
	float cellWidth = cellScale.x, cellHeight = cellScale.y;
	glm::vec2 cellPos(pos.x, pos.y);
	glm::vec2 pieceCenter;
	pieceCenter.x = ( - 1 + pos.x * radius) * cellWidth;
	pieceCenter.y = (- 1 + pos.y * radius) * cellHeight ;

	model = glm::translate(model, glm::vec3(pieceCenter, 1.0f));
	model = glm::scale(model, glm::vec3(radius, radius, 1.0f));

	//uniforms
	m_shader.uniformMat4(proj, "proj");
	m_shader.uniformMat4(model, "model");
	m_shader.uniform3f(col, "col");

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
	glBindVertexArray(0);

	glUseProgram(0);

}