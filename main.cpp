#include <iostream>
#include "GameState.h"
#include "movegenerator.h"
#include <cstdlib> 
#include "magic_generator.h"
#include "Evaluation.h"

using namespace std;
void testing();
void play();
void testing2();
void printMoves(const vector<Move>& moves);

int main ()
{
    init_magic();
    //testing();
    play();
    //testing2();
    return 0;
}

//starts a chess game
void play(){
    GameState game;
    Evaluation e;
    bool game_on = true;
    while (game_on) {
        vector<Move> legal_moves = MoveGenerator::generateLegalMoves(game);
        printMoves(legal_moves);
        game.bb.displayBoard();
        cout << "Score: " << e.evaluate(game.bb) << endl;
        string player_color = game.white_to_move ? "White: " : "Black: ";
        cout << player_color << "Enter your move (ex. e2e4), or 'quit' to exit: " << endl;

        string player_move;
        cin >> player_move;
        if (player_move == "quit") game_on = false;
        int from = (player_move[0] - 97) + (player_move[1] - 1) * 8;
        int to = (player_move[2] - 97) + (player_move[3] - 1) * 8;
        bool moving_own_piece = (game.bb.getWhitePieces() & (1ULL << from));
        bool moving_to_nonally_square = (!game.bb.getWhitePieces() & (1ULL << to));

        

        Move move(from, to);

        game.makeMove(move); 
    }
}

void testing() {
    cout << "hi this is testing stuff" << endl;
    //GameState game = GameState();
    GameState game("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    game.bb.displayBoard();

    MoveGenerator mg;
    vector<Move> moves = mg.generateLegalMoves(game);
    
    printMoves(moves);

    cout << game.exportFEN() << endl;
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
