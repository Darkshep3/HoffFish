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
        int en_passant; 
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
            //first split into the 6 parts
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
                fen_info[i]= fen_copy.substr(0, space_index);
                //cut down the part we stored
                fen_copy = fen_copy.substr(space_index + 1);
                //get a new index for the next space
                space_index = fen_copy.find(" ");
            }

            fen_info[5] = fen_copy;


            //now take piece placement and then separate line by line (separeted by / )
            string piece_placement[8];
            int slash_index = fen_info[0].find("/"); 
            for(int i = 0; slash_index != string::npos; i++)
            {
                
                //while we can find a slash
                piece_placement[i]= fen_info[0].substr(0, slash_index);
                //cut down the part we stored
                fen_info[0] = fen_info[0].substr(slash_index + 1);
                //get a new index for the next slash
                slash_index = fen_info[0].find("/");
            }

            piece_placement[7] = fen_info[0];

            //translate from fen to game state :d

            //pieces on the board (0)
            //loop thru each position and if it's not empty then set the bit
            //nested loop
            for(int rank = 0; rank < 8; rank++)
            {
                //increments the index for piece_placenment
                int file = 0;
                for(char current_char: piece_placement[rank])
                {
                    //for every character in piece_placement in specified rank,
                    if(isdigit(current_char))
                    {
                        //if current char is an int between 0 - 9
                        file += current_char - '0';
                        //increase the file by the # of emoty spaces on the board
                    }else
                    {
                        //there is actual piece on that square
                        int square = (7-rank) * 8 + file;
                        //7-rank bc fen begins on 8th rank and goes to 1st
                        set_bit(current_char, square);
                        //put that piece on the board
                        file++;
                    }
                    
                }
            }



            //active color (1)
            if(fen_info[1] == "w")
            {
                white_to_move = true;
            }else
            {
                white_to_move = false;
            }

            //castling rights (2)
            //set all to false
            //if it contains "-", leave it bc all is false
            //else, read in the chars and set corresponding bools to true

            castleWK = false;
            castleWQ = false;
            castleBK = false;
            castleBQ = false;

            if(fen_info[2] != "-")
            {
                for(int i = 0; i < fen_info[2].length();i++ )
                {
                    if(fen_info[2].at(i) == 'K')
                    {
                        castleWK = true;
                    }else if (fen_info[2].at(i) == 'Q')
                    {
                        castleWQ = true;
                    }else if (fen_info[2].at(i) == 'k')
                    {
                        castleBK = true;
                    }else if(fen_info[2].at(i) == 'q')
                    {
                        castleBQ = true;
                    }
                }
            }

            //en passant (3)
            //if no en passant, set int to -1
            if(fen_info[3] == "-")
            {
                en_passant = -1;
            }else
            {
                int file = fen_info[3].at(0) - 'a';
                int rank = fen_info[3].at(1) - 1;
                en_passant = rank*8 + file;
            }

            //half move (4)
            half_moves = stoi(fen_info[4]);

            //full move (5)
            full_moves = stoi(fen_info[5]);

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
        en_passant = 1ULL << square;
    } else {
        en_passant = 0ULL;
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
    if (en_passant) {
        int square = __builtin_ctzll(en_passant);
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
struct Move {
    int from;
    int to;
    char movedPiece;
    char capturedPiece;
    char promotionPiece; // new, for pawn promotion
    bool castleWK, castleWQ, castleBK, castleBQ;
    int en_passant;
    int half_moves;
    bool isEnPassant;
    bool isCastling;
};
void GameState::makeMove(int from, int to, char promotion = 0) 
{
    Move m;
    m.from = from;
    m.to = to;
    m.castleWK = castleWK;
    m.castleWQ = castleWQ;
    m.castleBK = castleBK;
    m.castleBQ = castleBQ;
    m.en_passant = en_passant;
    m.half_moves = half_moves;
    m.promotionPiece = promotion;
    m.isEnPassant = false;
    m.isCastling = false;

    char movedPiece = 0;
    char capturedPiece = 0;

    // Detect piece at source
    auto removePiece = [&](char piece) {
        switch(piece) {
            case 'P': bb.wpawns &= ~(1ULL << from); break;
            case 'N': bb.wknights &= ~(1ULL << from); break;
            case 'B': bb.wbishops &= ~(1ULL << from); break;
            case 'R': bb.wrooks &= ~(1ULL << from); break;
            case 'Q': bb.wqueens &= ~(1ULL << from); break;
            case 'K': bb.wking &= ~(1ULL << from); break;
            case 'p': bb.bpawns &= ~(1ULL << from); break;
            case 'n': bb.bknights &= ~(1ULL << from); break;
            case 'b': bb.bbishops &= ~(1ULL << from); break;
            case 'r': bb.brooks &= ~(1ULL << from); break;
            case 'q': bb.bqueens &= ~(1ULL << from); break;
            case 'k': bb.bking &= ~(1ULL << from); break;
        }
    };

    auto placePiece = [&](char piece, int square) {
        switch(piece) {
            case 'P': bb.wpawns |= (1ULL << square); break;
            case 'N': bb.wknights |= (1ULL << square); break;
            case 'B': bb.wbishops |= (1ULL << square); break;
            case 'R': bb.wrooks |= (1ULL << square); break;
            case 'Q': bb.wqueens |= (1ULL << square); break;
            case 'K': bb.wking |= (1ULL << square); break;
            case 'p': bb.bpawns |= (1ULL << square); break;
            case 'n': bb.bknights |= (1ULL << square); break;
            case 'b': bb.bbishops |= (1ULL << square); break;
            case 'r': bb.brooks |= (1ULL << square); break;
            case 'q': bb.bqueens |= (1ULL << square); break;
            case 'k': bb.bking |= (1ULL << square); break;
        }
    };

    // Find moved piece
    if (bb.wpawns & (1ULL << from)) movedPiece = 'P';
    else if (bb.wknights & (1ULL << from)) movedPiece = 'N';
    else if (bb.wbishops & (1ULL << from)) movedPiece = 'B';
    else if (bb.wrooks & (1ULL << from)) movedPiece = 'R';
    else if (bb.wqueens & (1ULL << from)) movedPiece = 'Q';
    else if (bb.wking & (1ULL << from)) movedPiece = 'K';
    else if (bb.bpawns & (1ULL << from)) movedPiece = 'p';
    else if (bb.bknights & (1ULL << from)) movedPiece = 'n';
    else if (bb.bbishops & (1ULL << from)) movedPiece = 'b';
    else if (bb.brooks & (1ULL << from)) movedPiece = 'r';
    else if (bb.bqueens & (1ULL << from)) movedPiece = 'q';
    else if (bb.bking & (1ULL << from)) movedPiece = 'k';

    // Detect capture
    for (char piece : {'P','N','B','R','Q','K','p','n','b','r','q','k'}) {
        int square = to;
        if (piece == 'P' && (bb.wpawns & (1ULL << square))) capturedPiece = 'P';
        else if (piece == 'N' && (bb.wknights & (1ULL << square))) capturedPiece = 'N';
        else if (piece == 'B' && (bb.wbishops & (1ULL << square))) capturedPiece = 'B';
        else if (piece == 'R' && (bb.wrooks & (1ULL << square))) capturedPiece = 'R';
        else if (piece == 'Q' && (bb.wqueens & (1ULL << square))) capturedPiece = 'Q';
        else if (piece == 'K' && (bb.wking & (1ULL << square))) capturedPiece = 'K';
        else if (piece == 'p' && (bb.bpawns & (1ULL << square))) capturedPiece = 'p';
        else if (piece == 'n' && (bb.bknights & (1ULL << square))) capturedPiece = 'n';
        else if (piece == 'b' && (bb.bbishops & (1ULL << square))) capturedPiece = 'b';
        else if (piece == 'r' && (bb.brooks & (1ULL << square))) capturedPiece = 'r';
        else if (piece == 'q' && (bb.bqueens & (1ULL << square))) capturedPiece = 'q';
        else if (piece == 'k' && (bb.bking & (1ULL << square))) capturedPiece = 'k';
    }

    // Handle en passant
    if ((movedPiece == 'P' || movedPiece == 'p') && to == en_passant) {
        m.isEnPassant = true;
        if (white_to_move) capturedPiece = 'p';
        else capturedPiece = 'P';
        int cap_square = white_to_move ? to - 8 : to + 8;
        removePiece(capturedPiece);
        to = to; // move normally
    }

    // Handle castling
    if (movedPiece == 'K' || movedPiece == 'k') {
        if (abs(to - from) == 2) { // castling detected
            m.isCastling = true;
            if (to == 6) { // white kingside
                removePiece('R'); bb.wrooks |= (1ULL << 5);
            } else if (to == 2) { // white queenside
                removePiece('R'); bb.wrooks |= (1ULL << 3);
            } else if (to == 62) { // black kingside
                removePiece('r'); bb.brooks |= (1ULL << 61);
            } else if (to == 58) { // black queenside
                removePiece('r'); bb.brooks |= (1ULL << 59);
            }
        }
    }

    // Remove piece from source
    removePiece(movedPiece);

    // Handle promotion
    if (promotion) {
        placePiece(promotion, to);
        m.promotionPiece = promotion;
    } else {
        placePiece(movedPiece, to);
    }

    m.movedPiece = movedPiece;
    m.capturedPiece = capturedPiece;
    moveHistory.push_back(m);

    // Update en passant for next move
    en_passant = -1;
    if (movedPiece == 'P' && from/8 == 1 && to/8 == 3) en_passant = from + 8;
    else if (movedPiece == 'p' && from/8 == 6 && to/8 == 4) en_passant = from - 8;

    white_to_move = !white_to_move;
}


void GameState::unmakeMove() {
    if (moveHistory.empty()) return;

    Move m = moveHistory.back();
    moveHistory.pop_back();

    // Remove moved piece from destination
    auto removePiece = [&](char piece, int square) {
        switch(piece) {
            case 'P': bb.wpawns &= ~(1ULL << square); break;
            case 'N': bb.wknights &= ~(1ULL << square); break;
            case 'B': bb.wbishops &= ~(1ULL << square); break;
            case 'R': bb.wrooks &= ~(1ULL << square); break;
            case 'Q': bb.wqueens &= ~(1ULL << square); break;
            case 'K': bb.wking &= ~(1ULL << square); break;
            case 'p': bb.bpawns &= ~(1ULL << square); break;
            case 'n': bb.bknights &= ~(1ULL << square); break;
            case 'b': bb.bbishops &= ~(1ULL << square); break;
            case 'r': bb.brooks &= ~(1ULL << square); break;
            case 'q': bb.bqueens &= ~(1ULL << square); break;
            case 'k': bb.bking &= ~(1ULL << square); break;
        }
    };

    auto placePiece = [&](char piece, int square) {
        switch(piece) {
            case 'P': bb.wpawns |= (1ULL << square); break;
            case 'N': bb.wknights |= (1ULL << square); break;
            case 'B': bb.wbishops |= (1ULL << square); break;
            case 'R': bb.wrooks |= (1ULL << square); break;
            case 'Q': bb.wqueens |= (1ULL << square); break;
            case 'K': bb.wking |= (1ULL << square); break;
            case 'p': bb.bpawns |= (1ULL << square); break;
            case 'n': bb.bknights |= (1ULL << square); break;
            case 'b': bb.bbishops |= (1ULL << square); break;
            case 'r': bb.brooks |= (1ULL << square); break;
            case 'q': bb.bqueens |= (1ULL << square); break;
            case 'k': bb.bking |= (1ULL << square); break;
        }
    };

    removePiece(m.promotionPiece ? m.promotionPiece : m.movedPiece, m.to);
    placePiece(m.movedPiece, m.from);

    // Restore captured piece
    if (m.capturedPiece) {
        if (m.isEnPassant) {
            int cap_square = white_to_move ? m.to - 8 : m.to + 8;
            placePiece(m.capturedPiece, cap_square);
        } else {
            placePiece(m.capturedPiece, m.to);
        }
    }

    // Undo castling
    if (m.isCastling) {
        if (m.to == 6) { // white kingside
            removePiece('R', 5); placePiece('R', 7);
        } else if (m.to == 2) { // white queenside
            removePiece('R', 3); placePiece('R', 0);
        } else if (m.to == 62) { // black kingside
            removePiece('r', 61); placePiece('r', 63);
        } else if (m.to == 58) { // black queenside
            removePiece('r', 59); placePiece('r', 56);
        }
    }

    // Restore castling, en passant, half_moves
    castleWK = m.castleWK;
    castleWQ = m.castleWQ;
    castleBK = m.castleBK;
    castleBQ = m.castleBQ;
    en_passant = m.en_passant;
    half_moves = m.half_moves;

    white_to_move = !white_to_move;
}
