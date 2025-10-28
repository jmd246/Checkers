#ifndef TILE
#define TILE

#include <optional>
#include <glm/glm.hpp>
#include "GamePiece.hpp"
#include <stdexcept>

class Tile {
private:
    glm::vec2 worldPos{ 0.0f, 0.0f };
    glm::vec3 color{ 1.0f, 1.0f, 1.0f };
    std::optional<GamePiece> piece;

public:
    Tile() = default;

    // --- Position and Color ---
    void setPos(const glm::vec2& pos) { worldPos = pos; }
    void setColor(const glm::vec3& c) { color = c; }

    const glm::vec2& getPos() const { return worldPos; }
    const glm::vec3& getColor() const { return color; }

    // --- Piece management ---
    bool isEmpty() const { return !piece.has_value(); }

    void setPiece(const GamePiece& p) { piece.emplace(p); }

    void removePiece() { piece.reset(); }

    GamePiece& getPiece() {
        if (!piece.has_value())
            throw std::runtime_error("Attempted to access piece from empty Tile");
        return *piece;
    }

    const GamePiece& getPiece() const {
        if (!piece.has_value())
            throw std::runtime_error("Attempted to access piece from empty Tile");
        return *piece;
    }

    // --- Optional access (non-throwing) ---
    const GamePiece* tryGetPiece() const {
        return piece ? &(*piece) : nullptr;
    }
};
#endif // !TILE
