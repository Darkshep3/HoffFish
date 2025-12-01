#include <iostream>
#include "GameState.h"
#include "movegenerator.h"
#include <cstdlib> 

using namespace std;
void testing();
void printMoves(const vector<Move>& moves);
string squareToAlgebraic(int sq);

int main ()
{
    testing();
    return 0;
}

//starts a chess game
void play(){

}

void testing() {
    cout << "hi this is testing stuff" << endl;
    //GameState game = GameState();
    GameState game("5k2/pp3pp1/3b2p1/6q1/1B1P4/7r/PPQ3P1/3R1RK1 b - - 1 27");
    game.bb.displayBoard();

    MoveGenerator mg;
    vector<Move> moves;
    U64 empty = game.bb.getEmpty();
    U64 white = game.bb.getWhitePieces();
    U64 black = game.bb.getBlackPieces();

    //mg.generateKingMoves(game.bb, white, empty, true, game.castleBK, game.castleBQ, moves);
    mg.generateKnightMoves(game.bb, white, true, moves);
    mg.generatePawnMoves(game.bb, white, empty, game.en_passant, true, moves);
    mg.generateBishopMoves(game.bb, white, empty, true, moves);
    printMoves(moves);
}

void printMoves(const vector<Move>& moves) {
    for (const Move& m : moves) {
        int from = m.getFromSquare();
        int to = m.getToSquare();
        cout << squareToAlgebraic(from) << squareToAlgebraic(to)<< endl;
    }
}

string squareToAlgebraic(int sq){
    if (sq < 0 || sq > 63){
        return "-";
    }
    char file = (char) ('a' + (sq % 8));
    char rank = (char) ('1' + (sq / 8));
    string s;
    s += file;
    s += rank;
    return s;
}