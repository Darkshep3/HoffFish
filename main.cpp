#include <iostream>
#include <cstdlib> 
#include "MagicGenerator.h"
#include <optional>
#include "Search.h"
#include <algorithm>
using namespace std;

const int SEARCH_DEPTH = 5;
void testing();
void play(GameState game, bool is_playing_against_engine, bool is_engine_white);
void testing2();
void print_moves(const vector<Move>& moves);

int main ()
{

    //Original Terminal Mode
    GameState game;
    //GameState game("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    //GameState game("r1bqkbnr/pppppppp/8/8/1nPP4/2N5/PP2PPPP/R1BQKBNR b KQkq - 2 3");
    init_magic();
    //testing();
    bool is_playing_against_engine = true;
    bool is_engine_white = false;
    play(game, is_playing_against_engine, is_engine_white);
    //testing2();i
    return 0;
}

optional<Move> parse_move(string input, GameState game) 
{
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
        char promo = tolower(input[5]);
        valid5 = (promo == 'q' || promo == 'r' || promo == 'b' || promo == 'n');
    } else {
        cout << "wrong input length: " << input.length() << "!= 4 or 6" << endl;
        return nullopt;
    }

    valid0 = (97 <= input[0] && input[0] <= 104);
    valid1 = (49 <= input[1] && input[1] <= 56);
    valid2 = (97 <= input[2] && input[2] <= 104);
    valid3 = (49 <= input[3] && input[3] <= 56);
    if (!(valid0 && valid1 && valid2 && valid3 && valid4 && valid5)) 
    {
        cout << "invalid string parse: " << valid0 << valid1 << valid2 << valid3 << valid4 << valid5 << endl;
        return nullopt;
    }
    int from = (input[0] - 97) + (input[1] - 49) * 8;
    int to = (input[2] - 97) + (input[3] - 49) * 8;
    // cout << "from = " << from << ", to = " << to << endl;
    bool moving_own_piece = game.white_to_move ? (game.bb.get_white_pieces() & (1ULL << from)) : (game.bb.get_black_pieces() & (1ULL << from));
    if (!moving_own_piece) {
        cout << "must move own piece" << endl;
        return nullopt;
    }
    
    Move move(from, to);

    if (input.length() == 6){
        char promo = game.white_to_move? toupper(input [5]) : tolower(input[5]);
        move.promotion = promo;
        move.move_type = move.get_promotion_move_type(promo);
    }

    vector<Move> legal_moves = MoveGenerator::generate_legal_moves(game);

    //Arush I changed ur ending because when u used return move ^ u lose whether or not it was MoveType::EnPassant as move(from, to) is always MoveType::NORMAL
    for (const Move& m : legal_moves){
        if (m.from == from && m.to == to &&
            m.promotion == move.promotion){
            return m;
        }
    }
    cout << "is not legal move" << endl;
    return nullopt;
}


bool is_game_over (GameState game) 
{
    //check if the current player has no legal moves or if the halfmove count is above 100 for 50-move rule
    vector<Move> legal_moves = MoveGenerator::generate_legal_moves(game);
    //print_moves(legal_moves);
    bool no_legal_moves = true;
    for (Move move : legal_moves) {
        if ((1ULL << move.get_from_square()) & (game.white_to_move ? game.bb.get_white_pieces() : game.bb.get_black_pieces())) {
            no_legal_moves = false;
        }
    }
    if (no_legal_moves) {
        if (MoveGenerator::is_in_check(game.white_to_move, game.bb)) {
            cout << (game.white_to_move ? "Black" : "White") << " wins by checkmate" << endl;
        } else {
            cout << "Draw by stalemate" << endl;
        }
        return true;
    } else if (game.half_moves >= 100) {
        cout << "Draw by 50-move rule" << endl;
        return true;
    }
    
    return false;
}


//starts a chess gamed
void play(GameState game, bool is_playing_against_engine, bool is_engine_white)
{
    Search engine = Search(1200);
    bool game_on = true;
    while (game_on) {
        // vector<Move> legal_moves = MoveGenerator::generateLegalMoves(game);
        // printMoves(legal_moves);
        game.bb.display_board();
        // cout << "Score: " << e.evaluate(game.bb) << endl;

        if (is_playing_against_engine && (game.white_to_move == is_engine_white)) {
            Move top_engine = engine.find_best_move(game, SEARCH_DEPTH);
            //cout << "Engine plays: " << top_engine.square_to_algebraic(top_engine.from) << top_engine.square_to_algebraic(top_engine.to) << endl;
            game.make_move(top_engine);
        } 
        else {
            string player_color = game.white_to_move ? "White: " : "Black: ";
            cout << player_color << "Enter your move (ex. e2e4), or 'quit' to exit: " << endl;

            string player_move;
            cin >> player_move;

            if (player_move == "quit") game_on = false;
            else {
                optional<Move> move = parse_move(player_move, game);
                if (move) {
                    game.make_move(*move);
                } else {
                    cout << "Invalid move, please try again." << endl;
                }
            }
        }
        if (is_game_over(game)) {
            game_on = false;
        }
        // int from = (player_move[0] - 97) + (player_move[1] - 1) * 8;
        // int to = (player_move[2] - 97) + (player_move[3] - 1) * 8;
        // bool moving_own_piece = (game.bb.getWhitePieces() & (1ULL << from));
        // bool moving_to_nonally_square = (!game.bb.getWhitePieces() & (1ULL << to));

        // Move move(from, to);
        // vector<Move> legal_moves = MoveGenerator::generateLegalMoves(game);
        // game.makeMove(move); 

    }
    game.bb.display_board();

}

void testing() 
{
    cout << "hi this is testing stuff" << endl;
    //GameState game = GameState();
    GameState game("r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1");
    game.bb.display_board();

    MoveGenerator mg;
    vector<Move> moves = mg.generate_legal_moves(game);
    
    print_moves(moves);

    cout << game.export_FEN() << endl;
}

void testing2()
{
    cout << "Magic Generator!!" << endl;
    magic_generator();
}

void print_moves(const vector<Move>& moves) 
{
    int count = 0;
    for (const Move& m : moves) 
    {
        count++;
        int from = m.get_from_square();
        int to = m.get_to_square();
        cout << count << ". " << m.square_to_algebraic(from) << m.square_to_algebraic(to);
        if (m.is_promotion())
            cout << "=" << char(toupper(m.promotion));
        cout << endl;
    }
}
