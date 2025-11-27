#pragma once
#include <cstdint>
#include <string>
enum class MoveType {
    NORMAL,
    CASTLING,
    EN_PASSANT,
    PROMOTION_QUEEN,
    PROMOTION_ROOK,
    PROMOTION_BISHOP,
    PROMOTION_KNIGHT,
    CAPTURES
};

class Move{
public:
    int from;
    int to;
    char capturedPiece;
    char promotionPiece;
    MoveType moveType;

    //the constructor
    Move(int from, int to)
    {
        this-> from = from;
        this-> to = to;
        moveType = MoveType::NORMAL;
        capturedPiece = ' ';
        promotionPiece = ' ';
    }

    Move(int from, int to, MoveType type)
    {
        this-> from = from;
        this-> to = to;
        this -> moveType = type;
        capturedPiece = ' ';
        promotionPiece = ' ';
    }

    Move(int from, int to, MoveType type, char promotion)
    {
        this-> from = from;
        this-> to = to;
        this-> moveType = type;
        capturedPiece = ' ';
        this-> promotionPiece = promotion;
    }


    int getFromSquare() const{
        return from;
    }
    int getToSquare() const {
        return to;
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


private:
    MoveType getPromotionMoveType(char promotion){
        switch(promotion){
            case 'q': return MoveType::PROMOTION_QUEEN;
            case 'r': return MoveType::PROMOTION_ROOK;
            case 'b': return MoveType::PROMOTION_BISHOP;
            case 'n': return MoveType::PROMOTION_KNIGHT;
            default: return MoveType::NORMAL;
        }
    }
};
