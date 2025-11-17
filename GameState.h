#pragma once
#include <string>
#include "Bitboard.h"
using namespace std;

class GameState{
    public: 
        Bitboard board;
        bool whiteToMove;
        bool castleWK, castleWQ;
        bool castleBK, castleBQ;
        U64 enPassants;
        int halfMovesSinceCaptureOrPawn;
        int fullMoves;
        // small task #1 Ethan 
        //... create the other variables we might need
        //hint: use the FEN notation we learned to see what is needed to represent a position

        //Constructor, a bit of object oriented stuff now
        GameState();
        //big task #2 Diana 
        explicit GameState(const string& fen); //Load from FEN string 
        //big task #3 Diana 

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

        void makeMove(int from, int to);
        void unmakeMove(); // medium task #6 Ethan 
        // finish the pawn move generation or try it out ig 

};