#pragma once 
#include "Move.h"

//Essentially gamestate but with all the functions 
//Stores only the important details for a chess game (FEN values)
class Delta {
public: 
    int from, to;
    char moved_piece;
    char captured_piece;
    char promotion_piece;

    bool white_to_move;
    bool castleWK, castleWQ;
    bool castleBK, castleBQ;
    int en_passant;
    int half_count;
    int full_count;

    MoveType move_type;

    //constructor
    Delta(int from, int to, char moved_piece, char captured_piece, char promotion_piece, bool white_to_move, bool castleWK,
        bool castleWQ, bool castleBK, bool castleBQ, int en_passant, int half_count, int full_count, MoveType move_type):
        from(from), 
        to(to), 
        moved_piece(moved_piece), 
        captured_piece(captured_piece), 
        promotion_piece(promotion_piece),
        white_to_move(white_to_move), 
        castleWK(castleWK), castleWQ(castleWQ), 
        castleBK(castleBK), castleBQ(castleBQ),
        en_passant(en_passant), 
        half_count(half_count), full_count(full_count), 
        move_type(move_type)
        {}
    };
