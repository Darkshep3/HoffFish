

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
static int clamp(int v, int lo, int hi) {
    return max(lo, min(v, hi));
}

// ---------------------------
// Convert UCI algebraic (e2e4, g7g8q) -> internal Move
// ---------------------------
optional<Move> parseMove(const string &input, GameState &game) {
    if (input.length() < 4) return nullopt;

    int fromFile = input[0] - 'a';
    int fromRank = input[1] - '1';
    int toFile   = input[2] - 'a';
    int toRank   = input[3] - '1';

    int from = fromRank * 8 + fromFile;
    int to   = toRank * 8 + toFile;

    char promo = '\0';
    if (input.length() == 5) {
        promo = toupper(input[4]);  // normalize promotion piece
    }

    // Generate legal moves
    auto legalMoves = MoveGenerator::generateLegalMoves(game);

    // Match move
    for (auto &m : legalMoves) {
        if (m.getFromSquare() == from && m.getToSquare() == to) {
            if (m.isPromotion()) {
                if (promo != '\0' && m.getPromotionPiece() == promo)
                    return m;
            } else {
                return m;
            }
        }
    }

    return nullopt;
}

// ---------------------------
// Parse UCI "position" command
// ---------------------------
static void parsePosition(const string &line) {
    stringstream ss(line);
    string token;
    ss >> token; // "position"
    ss >> token;

    if (token == "startpos") {
        game = GameState();
        if (ss >> token && token != "moves") return;
    } else if (token == "fen") {
        string fen, part;
        for (int i = 0; i < 6; i++) {
            ss >> part;
            fen += part + (i < 5 ? " " : "");
        }
        game = GameState(fen);
        if (ss >> token && token != "moves") return;
    }

    if (token == "moves") {
        while (ss >> token) {
            auto m = parseMove(token, game);
            if (!m) {
                cerr << "WARNING: Illegal move in UCI position: " << token << endl;
                continue;
            }
            game.makeMove(*m);
        }
    }
}

// ---------------------------
// UCI main loop
// ---------------------------
void uciLoop() {
    string line;
    while (getline(cin, line)) {
        if (line == "uci") {
            cout << "id name HoffFish\n";
            cout << "id author Allen Yang\n";
            cout << "option name Threads type spin default 1 min 1 max 64\n";
            cout << "option name Hash type spin default 64 min 1 max 1024\n";
            cout << "uciok\n" << flush;
        } 
        else if (line == "isready") {
            cout << "readyok\n" << flush;
        } 
        else if (line == "ucinewgame") {
            game = GameState();
        } 
        else if (line.rfind("position", 0) == 0) {
            parsePosition(line);
        } 
        else if (line.rfind("go", 0) == 0) {
            int depth = 64;
            int movetime = -1;
            int wtime = -1, btime = -1;
            int winc = 0, binc = 0;

            stringstream ss(line);
            string tok;
            ss >> tok; // "go"

            while (ss >> tok) {
                if (tok == "depth") ss >> depth;
                else if (tok == "movetime") ss >> movetime;
                else if (tok == "wtime") ss >> wtime;
                else if (tok == "btime") ss >> btime;
                else if (tok == "winc") ss >> winc;
                else if (tok == "binc") ss >> binc;
            }

            // Time allocation
            int timeForMove = 1000; // default 1s
            if (movetime != -1) timeForMove = movetime;
            else if (game.white_to_move && wtime != -1) timeForMove = wtime / 30 + winc;
            else if (!game.white_to_move && btime != -1) timeForMove = btime / 30 + binc;
            timeForMove = clamp(timeForMove, 50, 5000);

            engine.setTimeLimitMs(timeForMove);

            // Search
            Move best = engine.findBestMove(game, depth);

            // Output UCI move
            cout << "bestmove " 
                 << best.squareToAlgebraic(best.getFromSquare()) 
                 << best.squareToAlgebraic(best.getToSquare());
            if (best.isPromotion())
                cout << char(tolower(best.getPromotionPiece()));
            cout << "\n" << flush;

            // Make move internally
            game.makeMove(best);
        } 
        else if (line == "stop") {
            // ignored; search handles timeUp internally
        } 
        else if (line == "quit") {
            exit(0);
        }
    }
}
