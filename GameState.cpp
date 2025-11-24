#include <iostream>
#include <string>
#include <sstream>
#include "Bitboard.h"
#include "GameState.h"

using namespace std;

//write all the functions needed in GameState using gamestate.h's skeleton! 

class GameState{
    public: 
        Bitboard bb;
        bool white_to_move;
        bool castleWK, castleWQ;
        bool castleBK, castleBQ;
        int en_passants; 
        int full_moves, half_moves;
        //... create the other variables we might need
        //hint: use the FEN notation we learned to see what is needed to represent a position

        //Constructor, a bit of object oriented stuff now
        GameState()
        {
            white_to_move = true;
            castleWK = true;
            castleWQ = true;
            castleBK = true;
            castleBQ = true;
            int en_passant = -1;
            full_moves = 0;
            half_moves = 0;
            bb = Bitboard();
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


        };

void GameState::loadFEN(const string& str){
    //TODO: MUST CLEAR BITBOARD BEFORE RUNNING THIS CODE

    //Sample FEN: rnbqkbnr/ppp2ppp/4p3/3p4/2PP4/8/PP2PPPP/RNBQKBNR w KQkq d6 0 3

    //break FEN into sections by spaces
    istringstream strm;
    strm.str(str);

    string boardSection;
    char nextToMove;
    string castleSection;
    string enPassantSection;

    strm >> boardSection >> nextToMove >> castleSection >> enPassantSection >> half_moves >> full_moves;

    //set bitboard from board data
    int square = 56;
    for (int i=0; i<boardSection.length(); i++) {
        switch (boardSection[i]) {
            case 'r':
                bb.brooks |= (1ULL << square);
                square++;
                break;
            case 'n':
                bb.bknights |= (1ULL << square);
                square++;
                break;
            case 'b':
                bb.bbishops |= (1ULL << square);
                square++;
                break;
            case 'q':
                bb.bqueens |= (1ULL << square);
                square++;
                break;
            case 'k':
                bb.bking |= (1ULL << square);
                square++;
                break;
            case 'p':
                bb.bpawns |= (1ULL << square);
                square++;
                break; 
            case 'R':
                bb.wrooks |= (1ULL << square);
                square++;
                break;
            case 'N':
                bb.wknights |= (1ULL << square);
                square++;
                break;
            case 'B':
                bb.wbishops |= (1ULL << square);
                square++;
                break;
            case 'Q':
                bb.wqueens |= (1ULL << square);
                square++;
                break;
            case 'K':
                bb.wking |= (1ULL << square);
                square++;
                break;
            case 'P':
                bb.wpawns |= (1ULL << square);
                square++;
                break; 
            case '1':
                square += 1;
                break;
            case '2':
                square += 2;
                break;
            case '3':
                square += 3;
                break;
            case '4':
                square += 4;
                break;
            case '5':
                square += 5;
                break;
            case '6':
                square += 6;
                break;
            case '7':
                square += 7;
                break;
            case '8':
                square += 8;
                break;
            case '/':
                square -= 16;

        }
    }

    //set white to move or not
    white_to_move = (nextToMove == 'w');

    //set castling variables
    castleWK = (castleSection.find('K') != string::npos);
    castleWQ = (castleSection.find('Q') != string::npos);
    castleBK = (castleSection.find('k') != string::npos);
    castleBQ = (castleSection.find('q') != string::npos);

    //set en passant square (if it exists)
    if (enPassantSection != "-") {
        strm.clear();
        strm.str(enPassantSection);
        char file;
        int rank;
        strm >> file >> rank;

        square = (file - 97) + 8 * (rank - 1);
        en_passants = 1ULL << square;
    } else {
        en_passants = 0ULL;
    }
    
}

string GameState::exportFEN(){

    ostringstream strm;

    //write bitboard data to FEN format
    int emptySpaces = 0;
    for (int rank=7; rank>=0; rank--){
        for (int file=0; file<8; file++){
            int square = rank * 8 + file;

            if (!((bb.getBlackPieces() | bb.getWhitePieces()) & 1ULL << square)) {
                emptySpaces++;
            } else {
                if (emptySpaces != 0) {
                    strm << emptySpaces;
                }
                emptySpaces = 0;
            }

            if (bb.brooks & (1ULL << square)) strm << 'r';
            else if (bb.bknights & (1ULL << square)) strm << 'n';
            else if (bb.bbishops & (1ULL << square)) strm << 'b';
            else if (bb.bqueens & (1ULL << square)) strm << 'q';
            else if (bb.bking & (1ULL << square)) strm << 'k';
            else if (bb.bpawns & (1ULL << square)) strm << 'p';
            else if (bb.wrooks & (1ULL << square)) strm << 'R';
            else if (bb.wknights & (1ULL << square)) strm << 'N';
            else if (bb.wbishops & (1ULL << square)) strm << 'B';
            else if (bb.wqueens & (1ULL << square)) strm << 'Q';
            else if (bb.wking & (1ULL << square)) strm << 'K';
            else if (bb.wpawns & (1ULL << square)) strm << 'P';
        }

        if (emptySpaces != 0) {
            strm << emptySpaces;
        }
        emptySpaces = 0;

        if(rank != 0) {
            strm << '/';
        }

    }
    strm << ' ';

    //write next player to move
    if (white_to_move) {
        strm << 'w';
    } else {
        strm << 'b';
    }
    strm << ' ';

    //write castling 
    if (castleWK) strm << 'K';
    if (castleWQ) strm << 'Q';
    if (castleBK) strm << 'k';
    if (castleBQ) strm << 'q';
    if (!(castleWK | castleWQ | castleBK | castleBQ)) strm << '-';
    strm << ' ';

    //write en passant square (if it exists)
    if (en_passants) {
        int square = __builtin_ctzll(en_passants);
        int rank = square / 8; // 0-7
        int file = square % 8; // 0-7
        strm << char(file + 97) << (rank + 1);
    } else {
        strm << '-';
    }
    strm << ' ';

    //write half moves and full moves
    strm << half_moves << ' ' << full_moves;

    return strm.str();

}

void GameState::makeMove(int from, int to) {

    // move white pawns
    if (bb.wpawns & (1ULL << from)) {
        bb.wpawns &= ~(1ULL << from); // remove from source
        bb.wpawns |= (1ULL << to);    // place at destination
    }
    // move white knights
    else if (bb.wknights & (1ULL << from)) {
        bb.wknights &= ~(1ULL << from);
        bb.wknights |= (1ULL << to);
    }
    // move white bishops
    else if (bb.wbishops & (1ULL << from)) {
        bb.wbishops &= ~(1ULL << from);
        bb.wbishops |= (1ULL << to);
    }
    // move white rooks
    else if (bb.wrooks & (1ULL << from)) {
        bb.wrooks &= ~(1ULL << from);
        bb.wrooks |= (1ULL << to);
    }
    // move white queens
    else if (bb.wqueens & (1ULL << from)) {
        bb.wqueens &= ~(1ULL << from);
        bb.wqueens |= (1ULL << to);
    }
    // move white king
    else if (bb.wking & (1ULL << from)) {
        bb.wking &= ~(1ULL << from);
        bb.wking |= (1ULL << to);
    }
    // move black pawns
    else if (bb.bpawns & (1ULL << from)) {
        bb.bpawns &= ~(1ULL << from);
        bb.bpawns |= (1ULL << to);
    }
    // move black knights
    else if (bb.bknights & (1ULL << from)) {
        bb.bknights &= ~(1ULL << from);
        bb.bknights |= (1ULL << to);
    }
    // move black bishops
    else if (bb.bbishops & (1ULL << from)) {
        bb.bbishops &= ~(1ULL << from);
        bb.bbishops |= (1ULL << to);
    }
    // move black rooks
    else if (bb.brooks & (1ULL << from)) {
        bb.brooks &= ~(1ULL << from);
        bb.brooks |= (1ULL << to);
    }
    // move black queens
    else if (bb.bqueens & (1ULL << from)) {
        bb.bqueens &= ~(1ULL << from);
        bb.bqueens |= (1ULL << to);
    }
    // move black king
    else if (bb.bking & (1ULL << from)) {
        bb.bking &= ~(1ULL << from);
        bb.bking |= (1ULL << to);
    }
}

void GameState::unmakeMove(){

}

};
