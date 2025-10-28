#ifndef GAME_PIECE
#define GAME_PIECE
#include <Vector>
#include <glm/glm.hpp>

class GamePiece {
public:
	enum COAT {
		light, dark
	};
	enum RANK {
		pawn, king
	};
	void setCoat(COAT coat) {
		this->coat = coat;
	}
	void setRank(RANK rank) {
		this->rank = rank;
	}
	COAT getCoat() { 
		return coat; }
	RANK getRank() {
		return rank;
	}
	GamePiece(glm::vec2& position,COAT coat, RANK rank);
private:
	RANK rank;
	COAT coat;
	//list of vec2 possible moves;
	std::vector<glm::vec2> possibleMoves;

};
#endif //GAME_PIECE