#pragma once 
#include "Move.h"

class Delta {
public: 
    int from, to;
    char movedPiece;
    char capturedPiece;
    char promotionPiece;

    bool white_to_move;
    bool castleWK, castleWQ;
    bool castleBK, castleBQ;
    int enPassant;
    int halfCount;
    int fullCount;

    MoveType moveType;

    //constructor
    Delta(int from, int to, char movedPiece, char capturedPiece, char promotionPiece, bool white_to_move, bool castleWK,
        bool castleWQ, bool castleBK, bool castleBQ, int enPassant, int halfCount, int fullCount, MoveType movetype):
        from(from), 
        to(to), 
        movedPiece(movedPiece), 
        capturedPiece(capturedPiece), 
        promotionPiece(promotionPiece),
        white_to_move(white_to_move), 
        castleWK(castleWK), castleWQ(castleWQ), 
        castleBK(castleBK), castleBQ(castleBQ),
        enPassant(enPassant), 
        halfCount(halfCount), fullCount(fullCount), 
        moveType(moveType)
        {}
    };
