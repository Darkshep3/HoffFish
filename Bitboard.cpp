#include "Bitboard.h"
#include <iostream>
#include <string> 
using namespace std;

Bitboard::Bitboard(){
    wpawns = 0x0000000000000000ULL; // hexadecimal version 
    wknights = 0x0000000000000000ULL;
    wrooks = 0x0000000000000000ULL;
    wbishops = 0x0000000000000000ULL;
    wqueens = 0x0000000000000000ULL;
    wking = 0x0000000000000000ULL;
    bpawns = 0x0000000000000000ULL;
    bknights = 0x0000000000000000ULL;
    bbishops = 0x0000000000000000ULL;
    brooks = 0x0000000000000000ULL;
    bqueens = 0x0000000000000000ULL;
    bking = 0x0000000000000000ULL;
    updatePieceBb();
}

Bitboard::Bitboard(const Bitboard& other){
    wpawns = other.wpawns; 
    wknights = other.wknights;
    wrooks = other.wrooks;
    wbishops = other.wbishops;
    wqueens = other.wqueens;
    wking = other.wking;
    bpawns = other.bpawns;
    bknights = other.bknights;
    bbishops = other.bbishops;
    brooks = other.brooks;
    bqueens = other.bqueens;
    bking = other.bking;
    updatePieceBb();
}

void Bitboard::initialize(){
    wpawns = 0x000000000000FF00ULL; // hexadecimal version 
    wknights = 0x0000000000000042ULL;
    wrooks = 0x0000000000000081ULL;
    wbishops = 0x0000000000000024ULL;
    wqueens = 0x0000000000000008ULL;
    wking = 0x0000000000000010ULL;
    bpawns = 0x00FF000000000000ULL;
    bknights = 0x4200000000000000ULL;
    bbishops = 0x2400000000000000ULL;
    brooks = 0x8100000000000000ULL;
    bqueens = 0x0800000000000000ULL;
    bking = 0x1000000000000000ULL;
    updatePieceBb();
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

void Bitboard::clearSquare(int index) {
    U64 mask = ~(1ULL << index);
    wpawns &= mask;
    wknights &= mask;
    wbishops &= mask;
    wrooks &= mask;
    wqueens &= mask;
    wking &= mask;
    bpawns &= mask;
    bknights &= mask;
    bbishops &= mask;
    brooks &= mask;
    bqueens &= mask;
    bking &= mask;
    updatePieceBb();
}

void Bitboard::movePiece(int from, int to, char piece){
    if (piece == ' ') return;
    clearSquare(from);
    clearSquare(to);
    placePiece(piece, to);
}

U64 Bitboard::getWhitePieces(){
    return wpawns | wknights | wbishops | wrooks | wqueens | wking;
}

U64 Bitboard::getBlackPieces(){
    return bpawns | bknights | bbishops | brooks | bqueens | bking;
}

void Bitboard::placePiece(char piece, int index) {
        U64 bit = 1ULL << index;
        switch (piece) {
            case 'P':
                wpawns |= bit;
            break;
            case 'N': 
                wknights |= bit;
                break;
            case 'B': 
                wbishops |= bit;
                break;
            case 'R': 
                wrooks |= bit;
                break;
            case 'Q': 
                wqueens |= bit;
                break;
            case 'K': 
                wking |= bit;
                break;
            case 'p': 
                bpawns |= bit;
                break;
            case 'n': 
                bknights |= bit;
                break;
            case 'b': 
                bbishops |= bit;
                break;
            case 'r': 
                brooks |= bit;
                break;
            case 'q': 
                bqueens |= bit;
                break;
            case 'k': 
                bking |= bit;
                break;
        }
        updatePieceBb();
    }

    U64 Bitboard::getOccupied(){
        return getWhitePieces() | getBlackPieces();
    }
    U64 Bitboard::getEmpty(){
        return ~getOccupied();
    }
    char Bitboard::getPieceAt(int index) {
        U64 bit = 1ULL << index;
        for (int i = 0; i < 12; i++) {
            if ((pieceBb[i] & bit) != 0) {
                return pieceChars[i];
            }
        }
        return ' ';
    }
    void Bitboard::updatePieceBb() {
        pieceBb[0] = wpawns;
        pieceBb[1] = wknights;
        pieceBb[2] = wbishops;
        pieceBb[3] = wrooks;
        pieceBb[4] = wqueens;
        pieceBb[5] = wking;
        pieceBb[6] = bpawns;
        pieceBb[7] = bknights;
        pieceBb[8] = bbishops;
        pieceBb[9] = brooks;
        pieceBb[10] = bqueens;
        pieceBb[11] = bking;
    }
