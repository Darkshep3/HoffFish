#include "bitboard.h"
#include <iostream>
#include <string> 
using namespace std;

void Bitboard::initialize(){
    //wpawn initialization bitboard
    wpawns = 0x000000000000FF00L; // hexadecimal version 
    wknights = 0x0000000000000042L;
    wrooks = 0x0000000000000081L;
    wbishops = 0x0000000000000024L;
    wqueens = 0x0000000000000008L;
    wking = 0x0000000000000010L;
    bpawns = 0x00FF000000000000L;
    bknights = 0x4200000000000000L;
    bbishops = 0x2400000000000000L;
    brooks = 0x8100000000000000L;
    bqueens = 0x0800000000000000L;
    bking = 0x1000000000000000L;
}

void Bitboard::display (U64 bitboard){
    for (int rank = 7; rank >= 0; rank--){
        for (int file = 0; file < 8; file++){
            int square = rank * 8 + file;
            if ((bitboard & (1ULL << square)) != 0)  
                cout << 1 << " "; 
            else 
                cout << 0 << " ";
        }
        cout << endl; 
    
    }
    cout << endl;
}

void Bitboard::displayBoard(){
//pieces to print ♙♗♘♖♕♔♟♝♞♜♛♚
for (int rank = 7; rank >= 0; rank --){
    cout << rank + 1 << " "; 
    for (int file = 0; file < 8; file++){
        int square = rank * 8 + file;
        string piece = " "; 
        if ((wpawns & (1ULL << square)) != 0) piece = "♙"; 
        else if((wbishops & (1ULL << square)) != 0) piece = "♗";
        else if ((wknights & (1ULL << square)) != 0) piece = "♘";
        else if ((wrooks & (1ULL << square)) != 0) piece = "♖";
        else if ((wqueens & (1ULL << square)) != 0) piece = "♕";
        else if ((wking & (1ULL << square)) != 0) piece = "♔";
        else if ((bpawns & (1ULL << square)) != 0) piece = "♟";
        else if ((bknights & (1ULL << square)) != 0) piece = "♞";
        else if ((bbishops & (1ULL << square)) != 0) piece = "♝";
        else if ((brooks & (1ULL << square)) != 0) piece = "♜";
        else if ((bqueens & (1ULL << square)) != 0) piece = "♛";
        else if ((bking & (1ULL << square)) != 0) piece = "♚";
        cout << piece << " ";
    }
    cout << endl;
}
cout << "  a b c d e f g h" << endl;
}

void Bitboard::movePiece(U64 &bitboard, int from, int to){
    set_bit(bitboard, to);
    clear_bit(bitboard, from);
}

U64 Bitboard::getWhitePieces(){
    return wpawns | wknights | wbishops | wrooks | wqueens | wking;
}

U64 Bitboard::getBlackPieces(){
    return bpawns | bknights | bbishops | brooks | bqueens | bking;
}
