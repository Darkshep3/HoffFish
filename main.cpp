#include <iostream>
#include "GameState.h"
#include "movegenerator.h"
#include <cstdlib> 
#include "magic_generator.cpp"

using namespace std;
void testing();
void testing2();
void printMoves(const vector<Move>& moves);

int main ()
{
    init_magic();
    testing();
    //testing2();
    return 0;
}

//starts a chess game
void play(){

}

void testing() {
    cout << "hi this is testing stuff" << endl;
    //GameState game = GameState();
    GameState game("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    game.bb.displayBoard();

    MoveGenerator mg;
    vector<Move> moves;
    U64 empty = game.bb.getEmpty();
    U64 white = game.bb.getWhitePieces();
    U64 black = game.bb.getBlackPieces();

    //mg.generateKingMoves(game.bb, white, empty, true, game.castleBK, game.castleBQ, moves);
    mg.generateKnightMoves(game.bb, white, true, moves);
    mg.generatePawnMoves(game.bb, black, empty, game.en_passant, true, moves);
    mg.generateBishopMoves(game.bb, white, black, true, moves);
    mg.generateRookMoves(game.bb, white, black, true, moves);
    mg.generateQueenMoves(game.bb, white, black, true, moves);

    printMoves(moves);

    cout << game.exportFEN();
}

void testing2(){
    cout << "Magic Generator!!" << endl;
    magic_generator();
}

void printMoves(const vector<Move>& moves) {
    int count = 0;
    for (const Move& m : moves) {
        count++;
        int from = m.getFromSquare();
        int to = m.getToSquare();
        cout << count << ". " << m.squareToAlgebraic(from) << m.squareToAlgebraic(to)<< endl;
    }
}
