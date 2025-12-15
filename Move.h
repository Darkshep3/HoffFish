#pragma once
#include <cstdint>
#include <string>
enum class MoveType 
{
    NORMAL,
    CASTLING,
    EN_PASSANT,
    PROMOTION_QUEEN,
    PROMOTION_ROOK,
    PROMOTION_BISHOP,
    PROMOTION_KNIGHT,
    CAPTURES
};


class Move
{
public:
    int from;
    int to;
    char captured;
    char promotion;
    MoveType move_type;
    Move() : from (-1), to(-1), captured(' '), promotion(' '), move_type(MoveType::NORMAL){}

    //the constructor
    Move(int from, int to)
    {
        this-> from = from;
        this-> to = to;
        move_type = MoveType::NORMAL;
        captured = ' ';
        promotion = ' ';
    }

    Move(int from, int to, MoveType type)
    {
        this-> from = from;
        this-> to = to;
        this -> move_type = type;
        captured = ' ';
        promotion = ' ';
    }

    Move(int from, int to, MoveType type, char promotion)
    {
        this-> from = from;
        this-> to = to;
        this-> move_type = type;
        captured = ' ';
        this-> promotion = promotion;
    }


    int get_from_square() const{
        return from;
    }
    int get_to_square() const {
        return to;
    }

    char getCapturedPiece() const {
        return captured;
    }
    char get_promotion_piece() const{
        return promotion;
    }
    MoveType get_move_type() const {
        return move_type;
    }
    std::string square_to_algebraic(int sq) const{
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
    bool is_promotion() const{
        return move_type == MoveType::PROMOTION_QUEEN ||
            move_type == MoveType::PROMOTION_ROOK ||
            move_type == MoveType::PROMOTION_BISHOP ||
            move_type == MoveType::PROMOTION_KNIGHT;
    }

    bool operator==(const Move& other) const {
        return from == other.from && to == other.to;
    }

    MoveType get_promotion_move_type(char promotion){
        switch(promotion){
            case 'q': return MoveType::PROMOTION_QUEEN;
            case 'r': return MoveType::PROMOTION_ROOK;
            case 'b': return MoveType::PROMOTION_BISHOP;
            case 'n': return MoveType::PROMOTION_KNIGHT;
            default: return MoveType::NORMAL;
        }
    }
};
