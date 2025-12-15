#pragma once
#include "MoveGenerator.h"
#include "Evaluation.h"
#include <chrono> 

using namespace std;
using timer = std::chrono::system_clock;

constexpr int CHECKMATE_SCORE = 1000000;
constexpr int DRAW_SCORE = 0;

class Search
{
public: 
    Search();
    Search(int timeLimit);
    void set_search_depth(int depth);
    Move find_best_move(GameState& game, int max_depth);
    void set_time_limit_ms(int ms);
private:
    Move find_best_move_iterative(GameState& game, int max_depth);
    Move alpha_beta_root(GameState& gameState, int depth);
    double alpha_beta(GameState& game, int depth, double alpha, double beta, int max_depth);
    static void order_moves(vector<Move>& moves);
    static int get_move_priority(Move move);
    bool time_exceeded();
    int search_depth;
    timer::time_point start_time;
    int time_limit_ms;
    bool time_up;
};
