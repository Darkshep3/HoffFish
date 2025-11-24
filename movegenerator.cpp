#include "movegenerator.h"
bool MoveGenerator::isAttackedSquare(Bitboard& bb, int square, bool isWhite){

}
void MoveGenerator::generateKingMoves(Bitboard& bb, long allies, long empty, bool isWhiteToMove, bool kCastle, bool qCastle, vector<Move>& moves){
    U64 king = isWhiteToMove ? bb.wking : bb.bking;
    int fromSq = get_LSB(king);
    U64 temp = getKingMoves(fromSq, allies);
    while (temp != 0){
        moves.push_back(Move(fromSq, get_LSB(temp)));
    }
    if (kCastle) {
        int sq = isWhiteToMove? 5: 61;
        int sq2 = isWhiteToMove? 6: 62;
        bool f = (((1ULL << (sq)) & empty) != 0) && !(isAttackedSquare(bb, sq, !isWhiteToMove));
        bool g = ((1ULL << (sq2)) & empty) != 0 && !(isAttackedSquare(bb, sq2, !isWhiteToMove));
        if (f && g) {
            int toSq = isWhiteToMove? 6: 62;
            moves.push_back(Move(fromSq, toSq, MoveType::CASTLING));
        }
    }
    if (qCastle) {
        int sq = isWhiteToMove? 1: 57;
        int sq2 = isWhiteToMove? 2: 58;
        int sq3 = isWhiteToMove? 3: 59;
        bool b = ((1ULL << (sq)) & empty) != 0;
        bool c = ((1ULL << sq2) & empty) != 0 && !(isAttackedSquare(bb, sq2, !isWhiteToMove));
        bool d = ((1ULL << sq3) & empty) != 0 && !(isAttackedSquare(bb, sq3, !isWhiteToMove));

        if (b && c && d){
            int toSq = isWhiteToMove ? 2 : 58;
            moves.push_back(Move(fromSq, toSq, MoveType::CASTLING));
        }
    }
}

//Diana 
<<<<<<< HEAD
void MoveGenerator::generateKnightMoves(Bitboard& bb,  U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves){
=======
void MoveGenerator::generateKnightMoves(Bitboard& bb, long allies, bool isWhiteToMove, vector<Move>& moves)
{
    U64 knights = isWhiteToMove ? bb.wknights : bb.bknights;
    
    //loop thru each knight
    while(knights != 0)
    {
        int from_sq = get_LSB(knights);
        clear_LSB(knights);
        //remove current knight from bitboard
>>>>>>> refs/remotes/origin/main

        U64 temp = getKnightMoves(from_sq, allies);

        while(temp != 0)
        {
            int to_sq = get_LSB(temp);
            clear_LSB(temp);

            moves.push_back(Move(from_sq,to_sq));
        }
    }
}


//Ethan  

void MoveGenerator::generatePawnMoves(Bitboard& bb, U64 enemy, U64 empty, int enPassant, bool isWhiteToMove, vector<Move>& moves) {
    U64 pawns = isWhiteToMove ? bb.wpawns : bb.bpawns;
    int direction = isWhiteToMove ? 8 : -8; // white moves up, black moves down
    int startRank = isWhiteToMove ? 1 : 6;  // 2nd rank for white, 7th for black
    int promoRank = isWhiteToMove ? 6 : 1;  // 7th for white, 2nd for black

    while (pawns != 0) {
        int fromSq = get_LSB(pawns);
        clear_LSB(pawns);

        int rank = fromSq / 8;
        int file = fromSq % 8;

        int toSq = fromSq + direction;

        // Single move forward
        if (toSq >= 0 && toSq < 64 && ((1ULL << toSq) & (bb.getWhitePieces() | bb.getBlackPieces())) == 0) {
            // Promotion
            if (rank == promoRank) {
                moves.push_back(Move(fromSq, toSq, MoveType::PROMOTION, 0, isWhiteToMove ? 'Q' : 'q'));
                moves.push_back(Move(fromSq, toSq, MoveType::PROMOTION, 0, isWhiteToMove ? 'R' : 'r'));
                moves.push_back(Move(fromSq, toSq, MoveType::PROMOTION, 0, isWhiteToMove ? 'B' : 'b'));
                moves.push_back(Move(fromSq, toSq, MoveType::PROMOTION, 0, isWhiteToMove ? 'N' : 'n'));
            } else {
                moves.push_back(Move(fromSq, toSq, MoveType::NORMAL));
            }

            // Double move forward
            if (rank == startRank) {
                int doubleTo = fromSq + 2 * direction;
                if (((1ULL << doubleTo) & (bb.getWhitePieces() | bb.getBlackPieces())) == 0) {
                    moves.push_back(Move(fromSq, doubleTo, MoveType::DOUBLE_PAWN_PUSH));
                }
            }
        }

        // Captures
        int captureOffsets[2] = { direction + 1, direction - 1 }; // right and left diagonals
        for (int i = 0; i < 2; i++) {
            int capSq = fromSq + captureOffsets[i];
            if (capSq < 0 || capSq >= 64) continue;

            int capFile = capSq % 8;
            if (abs(capFile - file) != 1) continue; // avoid wrap-around (such as capturing from h file to a file)

            if ((1ULL << capSq) & enemy) { // normal capture
                if (rank == promoRank) { // promotion capture
                    moves.push_back(Move(fromSq, capSq, MoveType::PROMOTION, 0, isWhiteToMove ? 'Q' : 'q'));
                    moves.push_back(Move(fromSq, capSq, MoveType::PROMOTION, 0, isWhiteToMove ? 'R' : 'r'));
                    moves.push_back(Move(fromSq, capSq, MoveType::PROMOTION, 0, isWhiteToMove ? 'B' : 'b'));
                    moves.push_back(Move(fromSq, capSq, MoveType::PROMOTION, 0, isWhiteToMove ? 'N' : 'n'));
                } else {
                    moves.push_back(Move(fromSq, capSq, MoveType::CAPTURE));
                }
            }

            // En passant
            if (capSq == enPassant) {
                moves.push_back(Move(fromSq, capSq, MoveType::EN_PASSANT));
            }
        }
    }
}


//Arush 
//Bishop Move Generatoin 
// Rooks + queens if you have time  
void MoveGenerator::generateBishopMoves(Bitboard& bb, U64 enemy, U64 empty, bool isWhiteToMove, vector<Move>& moves){
    U64 bishops = isWhiteToMove ? bb.wbishops : bb.bbishops;
    vector<int> fromSqs;
    while (bishops != 0) {
        fromSqs.push_back(get_LSB(bishops));
        clear_bit(bishops, get_LSB(bishops));
    }

    for (int i=0; i<fromSqs.size(); i++) {
        int fromSq = fromSqs[i];
        int rank = fromSq / 8;
        int file = fromSq % 8;
        for (int j=-1; j<=1; j+=2) {
            for (int k=-1; k<=1; k+=2) {
                int mult = 1;
                int toRank = rank + j * mult;
                int toFile = file + k * mult; 
                while (toRank >= 0 && toRank <= 7 && toFile >= 0 && toFile <= 7) {
                    if (((1ULL << (toRank * 8 + toFile)) & enemy) != 0) {
                        moves.push_back(Move(fromSq, toRank * 8 + toFile, MoveType::CAPTURES));
                        toRank = 9;
                    } else if (((1ULL << (toRank * 8 + toFile)) & empty) != 0){
                        moves.push_back(Move(fromSq, toRank * 8 + toFile, MoveType::NORMAL));
                        mult++;
                        toRank = rank + j * mult;
                        toFile = file + k * mult;                         
                    } else {
                        toRank = 9;
                    }
                }
            }
        }
    }

}

