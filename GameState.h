#pragma once
#include <string>
#include "Bitboard.h"
#include "Move.h"
#include "Delta.h"
using namespace std;

class GameState{
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

        void loadFEN(const string& str);

        string exportFEN(); 

        void makeMove(Move move);
        void unmakeMove(const Delta& d); // medium task #6 Ethan 


};