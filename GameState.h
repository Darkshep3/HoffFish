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
        // small task #1 Ethan 
        //... create the other variables we might need
        //hint: use the FEN notation we learned to see what is needed to represent a position

        //Constructor, a bit of object oriented stuff now
        GameState(); //Diana
        explicit GameState(const string& fen); //Load from FEN string, Diana 
        //something new I read using explicit in C++
        //if u do GameState g = new GameState("FEN") returns an error as it returns a pointer GameState*
        //as a result, u should do GameState*g = new GameState("FEN") 
        // g-> board.displayBoard();
        //delete g; to manually free memory
        
        //a simpler way is using explicit where u can do this to declare a variable
        // GameState g("FEN");
        //g.board.displayBoard() and u dont need to delete memory 


        void loadFEN(const string& str); //big task #4 Arush
        //also apparently when i researched loadFEN (string str) creates a copy whenever u call it
        //therefore, passing by reference is more efficient cause no copy
        // but dont worry too much on these small details for now, we will finetune later! 

        string exportFEN(); // big task #5 Arush 

        void makeMove(Move move);
        void unmakeMove(const Delta& d); // medium task #6 Ethan 
        // finish the pawn move generation or try it out ig 

};