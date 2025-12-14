#include "movegenerator.h"
#include "Evaluation.h"
#include <chrono> 

using namespace std;
using timer = std::chrono::system_clock;

constexpr int CHECKMATE_SCORE = 1000000;
constexpr int DRAW_SCORE = 0;

class Search{
public: 
    Search();
    Search(int timeLimit);
    void setSearchDepth(int depth);
    Move findBestMove(GameState& game, int maxDepth);
private:
    Move findBestMoveIterative(GameState& game, int maxDepth);
    Move alphaBetaRoot(GameState& gameState, int depth);
    double alphabeta(GameState& game, int depth, double alpha, double beta, int maxDepth);
    static void orderMoves(vector<Move>& moves);
    static int getMovePriority(Move move);
    bool timeExceeded();
    int searchDepth;
    timer::time_point startTime;
    int timeLimitMs;
    bool timeUp;
};
