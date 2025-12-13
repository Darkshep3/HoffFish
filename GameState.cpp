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
    if (en_passant != -1) {
        int square = en_passant;
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

void GameState::makeMove(Move move) {
    int from = move.getFromSquare();
    int to = move.getToSquare();
    MoveType type = move.getMoveType();
    char piece = bb.getPieceAt(from);
    char captured = bb.getPieceAt(to);
    char promo = move.getPromotionPiece();

    // clear any en-passant from previous turn
    en_passant = -1;

    // remove captured piece first (except en passant, addressed later)
    if (captured != ' ' && type != MoveType::EN_PASSANT) {
        bb.clearSquare(to);
        half_moves = 0;   // capture resets halfmove counter
    }

    // en passant
    if (type == MoveType::EN_PASSANT) {
        int capSq = white_to_move ? (to - 8) : (to + 8);
        bb.clearSquare(capSq);
        half_moves = 0;
    }

    // half_move counter
    if (piece == 'P' || piece == 'p') {
        half_moves = 0;
    } else {
        half_moves++;
    }

    // full move counter
    if (!white_to_move) full_moves++;

    // move piece
    bb.clearSquare(from);

    // castling
    if (type == MoveType::CASTLING) {
        // king side
        if (to == from + 2) {
            if (white_to_move) {
                bb.clearSquare(7);
                bb.placePiece('R', 5);
            } else {
                bb.clearSquare(63);
                bb.placePiece('r', 61);
            }
        }
        // queen side
        else if (to == from - 2) {
            if (white_to_move) {
                bb.clearSquare(0);
                bb.placePiece('R', 3);
            } else {
                bb.clearSquare(56);
                bb.placePiece('r', 59);
            }
        }
    }

    // promotion
    if (move.isPromotion()) {
        bb.placePiece(promo, to);
    } else {
        bb.placePiece(piece, to);
    }

    // pawn double push: set en-passant square 
    if (piece == 'P' && from / 8 == 1 && to == from + 16) {
        en_passant = from + 8;
    }
    if (piece == 'p' && from / 8 == 6 && to == from - 16) {
        en_passant = from - 8;
    }

    // update castling rights
    if (piece == 'K') { castleWK = castleWQ = false; }
    if (piece == 'k') { castleBK = castleBQ = false; }

    if (from == 0 || to == 0) castleWQ = false;
    if (from == 7 || to == 7) castleWK = false;
    if (from == 56 || to == 56) castleBQ = false;
    if (from == 63 || to == 63) castleBK = false;

    // change sides to move
    white_to_move = !white_to_move;
}

void GameState::unmakeMove(const Delta &d) {

    // restore global state
    white_to_move = d.white_to_move;
    castleWK = d.castleWK;
    castleWQ = d.castleWQ;
    castleBK = d.castleBK;
    castleBQ = d.castleBQ;
    en_passant = d.enPassant;
    half_moves = d.halfCount;
    full_moves = d.fullCount;

    MoveType type = d.moveType;

    // remove piece from moved to square
    bb.clearSquare(d.to);

    // undo promotion
    if (type == MoveType::PROMOTION_QUEEN ||
        type == MoveType::PROMOTION_ROOK ||
        type == MoveType::PROMOTION_BISHOP ||
        type == MoveType::PROMOTION_KNIGHT) 
    {
        // restore pawn
        char pawn = (d.movedPiece == 'P' ? 'P' : 'p');
        bb.placePiece(pawn, d.from);
    }
    else {
        // restore piece on original square
        bb.placePiece(d.movedPiece, d.from);
    }

    // restore captured piece
    if (d.capturedPiece != ' ') {
        // normal capture
        if (type != MoveType::EN_PASSANT) {
            bb.placePiece(d.capturedPiece, d.to);
        }
        // en passant
        else {
            int capSq = (d.movedPiece == 'P' ? d.to - 8 : d.to + 8);
            bb.placePiece(d.capturedPiece, capSq);
        }
    }

    // undo castling
    if (type == MoveType::CASTLING) {
        if (d.movedPiece == 'K') {
            // king side
            if (d.to == d.from + 2) {
                bb.clearSquare(5);
                bb.placePiece('R', 7);
            }
            // queen side
            else if (d.to == d.from - 2) {
                bb.clearSquare(3);
                bb.placePiece('R', 0);
            }
        }
        else if (d.movedPiece == 'k') {
            if (d.to == d.from + 2) {
                bb.clearSquare(61);
                bb.placePiece('r', 63);
            }
            else if (d.to == d.from - 2) {
                bb.clearSquare(59);
                bb.placePiece('r', 56);
            }
        }
    }
}


//Makes a move but stores previous positional state
Delta GameState::deltaMove (Move move){
    int from = move.getFromSquare();
    int to = move.getToSquare();
    char piece = bb.getPieceAt(from);
    char captured;
    MoveType moveType = move.getMoveType();
    char promoPiece = move.getPromotionPiece();
    if (moveType == MoveType::EN_PASSANT){
        captured = white_to_move ? 'p' : 'P';
    }
    else if (move.isPromotion()){
        captured = bb.getPieceAt(to);
    }
    else {
        captured = bb.getPieceAt(to);
    }
    Delta delta = Delta(from, to, piece, captured, promoPiece, white_to_move, castleWK,
    castleWQ, castleBK, castleBQ, en_passant, half_moves, full_moves, moveType);
    makeMove(move);

    return delta;
};
    