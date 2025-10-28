#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui_impl_opengl3.h>
#include <imgui.h>
#include <Board.hpp>
#include <string>
std::string vertSrc, fragSrc;
int width(600), height(600);
void processInput(GLFWwindow* window);

void frameSizeCallBack(GLFWwindow* window,int width,int height) {
	glViewport(0, 0, width, height);
	// Retrieve your Board instance
	Board* board = static_cast<Board*>(glfwGetWindowUserPointer(window));
	if (board)
		board->setBoardSizeAndScale(width, width);
	else {
		std::cout << "failed";
	}
}
int main (){

	//initialize glfw
	glfwInit();

	//set window hints version major minor and core       
	glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);
	//set to compat for the old way of drawing quads
	//glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_COMPAT_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//create window
	GLFWwindow* window = glfwCreateWindow(width, height, "Checkers", NULL, NULL);
	//check if window was made
	if (!window) {
		std::cout << "Failed to create window";
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
	    glfwDestroyWindow(window);
		glfwTerminate();
		return -1;
	}
	Board board(8,8 );
	board.setUpBoard(vertSrc, fragSrc);
	glfwSetWindowUserPointer(window, &board);
	glfwSetFramebufferSizeCallback(window, frameSizeCallBack);
	board.setBoardSizeAndScale(width, height);
	board.initBuffers();

	// draw test pieces
	float tileRad = std::min(board.getTileDimenstions().x, board.getTileDimenstions().y) * 0.4f;
	glm::vec3 redCol(1.0f, 0.0f, 0.0f);
	glm::vec3 blackCol(0.0f, 0.0f, 0.0f);
	while (!glfwWindowShouldClose(window)) {
        
	   //process input
		processInput(window);
		//glEnable(GL_DEPTH_TEST);
		// render
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //red
		glClear(GL_COLOR_BUFFER_BIT);  // apply color

		board.draw();
		
		for (int i = 0; i < board.getWidth();i++) {
			
			for (int j = 0; j < board.getHeight();j++) {
				Tile tile = board.getTile(i, j);
				if (!tile.isEmpty()) {
					GamePiece piece = tile.getPiece();
					glm::vec3 color = piece.getCoat() == piece.light ? board.getPieceRenderer().lightColor : board.getPieceRenderer().darkColor;
					board.getPieceRenderer().draw(board.getTileDimenstions(), tile.getPos(), color, tileRad, board.getProj(), board.getModel());
				}
			}
		}

	   // poll events
		glfwPollEvents();
	   //swap buffers
		glfwSwapBuffers(window);																																																									
	
	}


	return 0;
}



void processInput(GLFWwindow* window) {
	if ( glfwGetKey(window,GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window,true);
	}

}

