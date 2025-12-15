#pragma once
#include <string>
#include "Bitboard.h"
#include "Move.h"
#include "Delta.h"
using namespace std;

class GameState
{
    public: 
        Bitboard bb;
        bool white_to_move;
        bool castleWK, castleWQ;
        bool castleBK, castleBQ;
        int en_passant;
        int half_moves;
        int full_moves;
        
        //Constructor
        GameState(); //Diana
        GameState(const string& fen); //Load from FEN string, Diana 

        string export_FEN(); 

        void make_move(Move move);
        void unmake_move(const Delta& d); // medium task #6 Ethan 
        Delta delta_move(Move move); //essentially runs make move but leaves behind the copy of the original position 

};