#include <iostream>
#include "GameState.h"
#include "movegenerator.h"
#include <cstdlib> 
#include "magic_generator.h"
#include <optional>

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

optional<Move> parseMove(string input, GameState game) {
    bool valid0; 
    bool valid1;
    bool valid2;
    bool valid3;
    bool valid4;
    bool valid5;

    if (input.length() == 4) {
        valid4 = true;
        valid5 = true;
    } else if (input.length() == 6) {
        valid4 = (input[4] == '=');
        valid5 = (input[5] == 'q' || input[5] == 'r' || input[5] == 'b' || input[5] == 'k');
    } else {
        return;
    }

    valid0 = (97 <= input[0] && input[0] <= 104);
    valid1 = (49 <= input[1] && input[1] <= 56);
    valid2 = (97 <= input[2] && input[2] <= 104);
    valid3 = (49 <= input[3] && input[3] <= 56);
    if (!(valid0 && valid1 && valid2 && valid3 && valid4 && valid5)) {
        return;
    }
    int from = (input[0] - 97) + (input[1] - 1) * 8;
    int to = (input[2] - 97) + (input[3] - 1) * 8;
    bool moving_own_piece = (game.bb.getWhitePieces() & (1ULL << from));
    if (!moving_own_piece) {
        return;
    }
    
    Move move(from, to);

    vector<Move> legal_moves = MoveGenerator::generateLegalMoves(game);
    bool is_legal_move = (find(legal_moves.begin(), legal_moves.end(), move) != legal_moves.end());

    if (!is_legal_move) {
        return;
    }

    return move;

}


//starts a chess game
void play(){
    GameState game;
    bool game_on = true;
    while (game_on) {
        game.bb.displayBoard();
        string player_color = game.white_to_move ? "White: " : "Black: ";
        cout << player_color << "Enter your move (ex. e2e4), or 'quit' to exit: " << endl;

        string player_move;
        cin >> player_move;

        if (player_move == "quit") game_on = false;
        else {
            optional<Move> move = parseMove(player_move, game);
            if (move) {
                game.makeMove(*move);
            } else {
                cout << "Invalid move, please try again." << endl;
            }
        }
        // int from = (player_move[0] - 97) + (player_move[1] - 1) * 8;
        // int to = (player_move[2] - 97) + (player_move[3] - 1) * 8;
        // bool moving_own_piece = (game.bb.getWhitePieces() & (1ULL << from));
        // bool moving_to_nonally_square = (!game.bb.getWhitePieces() & (1ULL << to));

        

        // Move move(from, to);
        // vector<Move> legal_moves = MoveGenerator::generateLegalMoves(game);
        // game.makeMove(move); 
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
