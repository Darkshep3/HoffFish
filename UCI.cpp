

#include "Search.h"
#include "MoveGenerator.h"
#include "GameState.h"
#include "Move.h"
#include <iostream>
#include <sstream>
#include <string>
#include <optional>
#include <algorithm>

using namespace std;

// Global engine state
static GameState game;
static Search engine;

// ---------------------------
// Helper: clamp value
// ---------------------------
static int clamp(int v, int lo, int hi) 
{
    return max(lo, min(v, hi));
}

// ---------------------------
// Convert UCI algebraic (e2e4, g7g8q) -> internal Move
// ---------------------------
optional<Move> parse_move(const string &input, GameState &game) 
{
    if (input.length() < 4) return nullopt;

    int from_file = input[0] - 'a';
    int from_rank = input[1] - '1';
    int to_file   = input[2] - 'a';
    int to_rank   = input[3] - '1';

    int from = from_rank * 8 + from_file;
    int to   = to_rank * 8 + to_file;

    char promo = '\0';
    if (input.length() == 5) 
    {
        promo = toupper(input[4]);  // normalize promotion piece
    }

    // Generate legal moves
    auto legalMoves = MoveGenerator::generate_legal_moves(game);

    // Match move
    for (auto &m : legalMoves) 
    {
        if (m.get_from_square() == from && m.get_to_square() == to) 
        {
            if (m.is_promotion()) 
            {
                if (promo != '\0' && m.get_promotion_piece() == promo)
                    return m;
            } else 
            {
                return m;
            }
        }
    }

    return nullopt;
}

// ---------------------------
// Parse UCI "position" command
// ---------------------------
static void parse_position(const string &line) 
{
    stringstream ss(line);
    string token;
    ss >> token; // "position"
    ss >> token;

    if (token == "startpos") 
    {
        game = GameState();
        if (ss >> token && token != "moves") return;
    } else if (token == "fen") 
    {
        string fen, part;
        for (int i = 0; i < 6; i++) 
        {
            ss >> part;
            fen += part + (i < 5 ? " " : "");
        }
        game = GameState(fen);
        if (ss >> token && token != "moves") return;
    }

    if (token == "moves") 
    {
        while (ss >> token) 
        {
            auto m = parse_move(token, game);
            if (!m) 
            {
                cerr << "WARNING: Illegal move in UCI position: " << token << endl;
                continue;
            }
            game.make_move(*m);
        }
    }
}

// ---------------------------
// UCI main loop
// ---------------------------
void uci_loop() {
    string line;
    while (getline(cin, line)) 
    {
        if (line == "uci") {
            cout << "id name HoffFish\n";
            cout << "id author Allen Yang\n";
            cout << "option name Threads type spin default 1 min 1 max 64\n";
            cout << "option name Hash type spin default 64 min 1 max 1024\n";
            cout << "uciok\n" << flush;
        } 
        else if (line == "isready") 
        {
            cout << "readyok\n" << flush;
        } 
        else if (line == "ucinewgame") 
        {
            game = GameState();
        } 
        else if (line.rfind("position", 0) == 0) 
        {
            parse_position(line);
        } 
        else if (line.rfind("go", 0) == 0) 
        {
            int depth = 64;
            int movetime = -1;
            int wtime = -1, btime = -1;
            int winc = 0, binc = 0;

            stringstream ss(line);
            string tok;
            ss >> tok; // "go"

            while (ss >> tok) 
            {
                if (tok == "depth") ss >> depth;
                else if (tok == "movetime") ss >> movetime;
                else if (tok == "wtime") ss >> wtime;
                else if (tok == "btime") ss >> btime;
                else if (tok == "winc") ss >> winc;
                else if (tok == "binc") ss >> binc;
            }

            // Time allocation
            int time_for_move = 1000; // default 1s
            if (movetime != -1) time_for_move = movetime;
            else if (game.white_to_move && wtime != -1) time_for_move = wtime / 30 + winc;
            else if (!game.white_to_move && btime != -1) time_for_move = btime / 30 + binc;
            time_for_move = clamp(time_for_move, 50, 5000);

            engine.set_time_limit_ms(time_for_move);

            // Search
            Move best = engine.find_best_move(game, depth);

            // Output UCI move
            cout << "bestmove " 
                 << best.square_to_algebraic(best.get_from_square()) 
                 << best.square_to_algebraic(best.get_to_square());
            if (best.is_promotion())
                cout << char(tolower(best.get_promotion_piece()));
            cout << "\n" << flush;

            // Make move internally
            game.make_move(best);
        } 
        else if (line == "stop") 
        {
            // ignored; search handles timeUp internally
        } 
        else if (line == "quit") 
        {
            exit(0);
        }
    }
}
