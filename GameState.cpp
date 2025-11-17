#include <iostream>
#include <string>
#include <sstream>
#include "Bitboard.h"
#include "GameState.h"

using namespace std;

//write all the functions needed in GameState using gamestate.h's skeleton! 

GameState::GameState(){

}

GameState::GameState(const string& fen){

} 

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

    strm >> boardSection >> nextToMove >> castleSection >> enPassantSection >> halfMovesSinceCaptureOrPawn >> fullMoves;

    //set bitboard from board data
    int square = 56;
    for (int i=0; i<boardSection.length(); i++) {
        switch (boardSection[i]) {
            case 'r':
                board.brooks |= (1ULL << square);
                square++;
                break;
            case 'n':
                board.bknights |= (1ULL << square);
                square++;
                break;
            case 'b':
                board.bbishops |= (1ULL << square);
                square++;
                break;
            case 'q':
                board.bqueens |= (1ULL << square);
                square++;
                break;
            case 'k':
                board.bking |= (1ULL << square);
                square++;
                break;
            case 'p':
                board.bpawns |= (1ULL << square);
                square++;
                break; 
            case 'R':
                board.wrooks |= (1ULL << square);
                square++;
                break;
            case 'N':
                board.wknights |= (1ULL << square);
                square++;
                break;
            case 'B':
                board.wbishops |= (1ULL << square);
                square++;
                break;
            case 'Q':
                board.wqueens |= (1ULL << square);
                square++;
                break;
            case 'K':
                board.wking |= (1ULL << square);
                square++;
                break;
            case 'P':
                board.wpawns |= (1ULL << square);
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
    whiteToMove = (nextToMove == 'w');

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
        enPassants = 1ULL << square;
    } else {
        enPassants = 0ULL;
    }
    
}

string GameState::exportFEN(){

    ostringstream strm;

    //write bitboard data to FEN format
    int emptySpaces = 0;
    for (int rank=7; rank>=0; rank--){
        for (int file=0; file<8; file++){
            int square = rank * 8 + file;

            if (!((board.getBlackPieces() | board.getWhitePieces()) & 1ULL << square)) {
                emptySpaces++;
            } else {
                if (emptySpaces != 0) {
                    strm << emptySpaces;
                }
                emptySpaces = 0;
            }

            if (board.brooks & (1ULL << square)) strm << 'r';
            else if (board.bknights & (1ULL << square)) strm << 'n';
            else if (board.bbishops & (1ULL << square)) strm << 'b';
            else if (board.bqueens & (1ULL << square)) strm << 'q';
            else if (board.bking & (1ULL << square)) strm << 'k';
            else if (board.bpawns & (1ULL << square)) strm << 'p';
            else if (board.wrooks & (1ULL << square)) strm << 'R';
            else if (board.wknights & (1ULL << square)) strm << 'N';
            else if (board.wbishops & (1ULL << square)) strm << 'B';
            else if (board.wqueens & (1ULL << square)) strm << 'Q';
            else if (board.wking & (1ULL << square)) strm << 'K';
            else if (board.wpawns & (1ULL << square)) strm << 'P';
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
    if (whiteToMove) {
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
    if (enPassants) {
        int square = __builtin_ctzll(enPassants);
        int rank = square / 8; // 0-7
        int file = square % 8; // 0-7
        strm << char(file + 97) << (rank + 1);
    } else {
        strm << '-';
    }
    strm << ' ';

    //write half moves and full moves
    strm << halfMovesSinceCaptureOrPawn << ' ' << fullMoves;

    return strm.str();

}

void GameState::makeMove(int from, int to){

}

void GameState::unmakeMove(){

}