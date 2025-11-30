#include <iostream>
#include <string>
#include <sstream>
#include "GameState.h"
#include "Bitboard.h"

using namespace std;

//write all the functions needed in GameState using gamestate.h's skeleton! 

//Constructor, a bit of object oriented stuff now
GameState::GameState()
{
    white_to_move = true;
    castleWK = true;
    castleWQ = true;
    castleBK = true;
    castleBQ = true;
    en_passant = -1;
    full_moves = 0;
    half_moves = 0;
    bb.initialize();
}; 
//Diana
GameState::GameState(const string& fen)
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

    //pieces on the bb (0)
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
                //increase the file by the # of emoty spaces on the bb
            }else
            {
                //there is actual piece on that square
                int square = (7-rank) * 8 + file;
                //7-rank bc fen begins on 8th rank and goes to 1st

                //set_bit(current_char, square);
                bb.placePiece(current_char, square);

                //put that piece on the bb
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

    string bbSection;
    char nextToMove;
    string castleSection;
    string enPassantSection;

    strm >> bbSection >> nextToMove >> castleSection >> enPassantSection >> half_moves >> full_moves;

    //set bitbb from bb data
    int square = 56;
    for (int i=0; i<bbSection.length(); i++) {
        switch (bbSection[i]) {
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

    //write bitbb data to FEN format
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

void GameState::makeMove(const Move& m, Delta& d) {
    char piece = bb[m.from];
    char target = bb[m.to];

    // save state to Delta
    d.from = m.from;
    d.to = m.to;
    d.movedPiece = piece;
    d.capturedPiece = target;
    d.promotionPiece = m.promotion;

    d.white_to_move = white_to_move;

    d.castleWK = castleWK;
    d.castleWQ = castleWQ;
    d.castleBK = castleBK;
    d.castleBQ = castleBQ;

    d.enPassant = en_passant;
    d.halfCount = half_moves;
    d.fullCount = full_moves;

    // apply move
    // Clear from-square
    bb[mv.from] = ' ';

    bool isWhite = (piece >= 'A' && piece <= 'Z');

    // Reset en passant
    en_passant = -1;

    // castling
    if ((piece == 'K' && m.from == 60 && (m.to == 62 || m.to == 58)) ||
        (piece == 'k' && m.from == 4 && (m.to == 6 || m.to == 2))) {

        // King moves two squares
        if (mv.to == m.from + 2) { // king-side
            // move the rook
            bb[m.from + 1] = bb[m.from + 3];
            bb[m.from + 3] = ' ';
        } else { // queen-side
            bb[m.from - 1] = bb[m.from - 4];
            bb[m.from - 4] = ' ';
        }
    }

    // en passant
    if ((piece == 'P' || piece == 'p')
        && m.to == en_passant) {

        int capSq = isWhite ? (m.to + 8) : (m.to - 8);
        d.capturedPiece = bb[capSq];
        bb[capSq] = ' ';
    }

    // promotion
    if (m.promotion) {
        bb[m.to] = m.promotion;
    } else {
        bb[m.to] = piece;
    }

    // set new en passant square
    if (piece == 'P') {
        if (m.to == m.from - 16) en_passant = m.from - 8;
    }
    if (piece == 'p') {
        if (m.to == m.from + 16) en_passant = m.from + 8;
    }

    // update castling
    if (piece == 'K') {
        castleWK = castleWQ = false;
    }
    if (piece == 'k') {
        castleBK = castleBQ = false;
    }

    if (m.from == 63 || m.to == 63) castleWK = false;
    if (m.from == 56 || m.to == 56) castleWQ = false;
    if (m.from == 7  || m.to == 7)  castleBK = false;
    if (m.from == 0  || m.to == 0)  castleBQ = false;

    // move counters
    if (piece == 'P' || piece == 'p' || target != ' ')
        half_moves = 0;
    else
        half_moves++;

    if (!white_to_move)
        full_moves++;

    // switch white to black and vice versa
    white_to_move = !white_to_move;
}

void GameState::unmakeMove(const Delta& d) {
    // restore side to move and counters
    white_to_move = d.white_to_move;

    castleWK = d.castleWK;
    castleWQ = d.castleWQ;
    castleBK = d.castleBK;
    castleBQ = d.castleBQ;

    en_passant = d.enPassant;
    half_moves = d.halfCount;
    full_moves = d.fullCount;

    //restore bb
    bb[d.from] = d.movedPiece;
    bb[d.to] = d.capturedPiece;

    char piece = d.movedPiece;

    // undo promotion
    if (d.promotionPiece) {
        bb[d.from] = (white_to_move ? 'P' : 'p');
    }

    // undo en passant
    if ((piece == 'P' || piece == 'p')
        && d.to == d.enPassant
        && d.capturedPiece == ' ') {

        bool isWhite = (piece == 'P');
        int capSq = isWhite ? (d.to + 8) : (d.to - 8);
        bb[capSq] = isWhite ? 'p' : 'P';
    }

    // undo castling
    if ((piece == 'K' && d.from == 60 && (d.to == 62 || d.to == 58)) ||
        (piece == 'k' && d.from == 4 && (d.to == 6 || d.to == 2))) {

        if (d.to == d.from + 2) { // king-side
            bb[d.from + 3] = bb[d.from + 1];
            bb[d.from + 1] = ' ';
        } else { // queen-side
            bb[d.from - 4] = bb[d.from - 1];
            bb[d.from - 1] = ' ';
        }
    }
}

