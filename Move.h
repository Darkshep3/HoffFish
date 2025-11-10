#pragma once
#include <cstdint>

enum class MoveType {
    NORMAL,
    CASTLING,
    EN_PASSSANT,
    PROMOTION_QUEEN,
    PROMOTION_ROOK,
    PROMOTION_BISHOP,
    PROMOTION_KNIGHT,
    CAPTURES
};

class Move{
public:
    int fromSquare;
    int toSquare;
    char capturedPiece;
    char promotionPiece;
    MoveType moveType;

    //the constructor
    Move(int from, int to, MoveType type, char promotion);

    int getFromSquare() const{
        return fromSquare;
    }
    int getToSquare() const {
        return toSquare;
    }

    char getCapturedPiece() const {
        return capturedPiece;
    }
    char getPromotionPiece() const{
        return promotionPiece;
    }
    MoveType getMoveType() const {
        return moveType;
    }
};