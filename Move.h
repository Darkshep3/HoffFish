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
    char captured;
    char promotion;
    MoveType moveType;


    //the constructor
    Move(int from, int to)
    {
        this-> from = from;
        this-> to = to;
        moveType = MoveType::NORMAL;
        captured = ' ';
        promotion = ' ';
    }

    Move(int from, int to, MoveType type)
    {
        this-> from = from;
        this-> to = to;
        this -> moveType = type;
        captured = ' ';
        promotion = ' ';
    }

    Move(int from, int to, MoveType type, char promotion)
    {
        this-> from = from;
        this-> to = to;
        this-> moveType = type;
        captured = ' ';
        this-> promotion = promotion;
    }


    int getFromSquare() const{
        return from;
    }
    int getToSquare() const {
        return to;
    }

    char getCapturedPiece() const {
        return captured;
    }
    char getPromotionPiece() const{
        return promotion;
    }
    MoveType getMoveType() const {
        return moveType;
    }
    std::string squareToAlgebraic(int sq) const{
        if (sq < 0 || sq > 63){
            return "-";
        }
        char file = (char) ('a' + (sq % 8));
        char rank = (char) ('1' + (sq / 8));
        std::string s;
        s += file;
        s += rank;
        return s;
    }
    bool isPromotion() {
        return moveType == MoveType::PROMOTION_QUEEN ||
            moveType == MoveType::PROMOTION_ROOK ||
            moveType == MoveType::PROMOTION_BISHOP ||
            moveType == MoveType::PROMOTION_KNIGHT;
    }

    bool operator==(const Move& other) const {
        return (from == other.from && to == other.to);
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
