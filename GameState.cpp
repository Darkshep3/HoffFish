#pragma once
#include <iostream>
#include "Bitboard.h"
#include <string>
using namespace std;

//write all the functions needed in GameState using gamestate.h's skeleton! 
class GameState{
    public: 
        Bitboard board;
        bool whiteToMove;
        bool castleWK, castleWQ;
        bool castleBK, castleBQ;
        int en_passant; 
        int full_moves, half_moves;
        //... create the other variables we might need
        //hint: use the FEN notation we learned to see what is needed to represent a position

        //Constructor, a bit of object oriented stuff now
        GameState()
        {
            whiteToMove = true;
            castleWK = true;
            castleWQ = true;
            castleBK = true;
            castleBQ = true;
            int en_passant = -1;
            full_moves = 0;
            half_moves = 0;
            board = new Bitboard();
        }; //Diana
        explicit GameState(const string fen)
        {
            string fen_copy = fen;
            int length = fen.length();
            string fen_info[6];
            //the 6 pieces of info are:
            // string piece_placement;
            // string active_color;
            // string castling_rights;
            // string en_passant;
            // string half_moves;
            // string full_moves;
            int space_index = fen_copy.find(" "); 
            for(int i = 0; space_index != string::npos; i++)
            {
                
                //while we can find a space
                fen_info[i]= fen_copy.substr(0, space_index + 1);
                //cut down the part we stored
                fen_copy = fen_copy.substr(space_index + 1, length - (space_index + 1));
                //get a new index for the next space
                space_index = fen_copy.find(" ");
            }
            
            //translate from fen to game state

            //pieces on the board

            //active color

            //castling rights

            //en passant

            //half move

            //full move


        }; //Load from FEN string, Diana 
        //something new I read using explicit in C++
        //if u do GameState g = new GameState("FEN") returns an error as it returns a pointer GameState*
        //as a result, u should do GameState*g = new GameState("FEN") 
        // g-> board.displayBoard();
        //delete g; to manually free memory
        
        //a simpler way is using explicit where u can do this to declare a variable
        // GameState g("FEN");
        //g.board.displayBoard() and u dont need to delete memory 


        void loadFEN(const string& str);
        //also apparently when i researched loadFEN (string str) creates a copy whenever u call it
        //therefore, passing by reference is more efficient cause no copy
        // but dont worry too much on these small details for now, we will finetune later! 

        string exportFEN();

        void makeMove(int from, int to);
        void unmakeMove();

};