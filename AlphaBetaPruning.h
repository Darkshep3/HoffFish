#include "Move.h"
#include "GameState.h"
#include "movegenerator.h"
#include "Evaluation.h"
using namespace std;

struct SearchResult {
    double value;
    Move move;
};

SearchResult alphabeta(GameState game, int depth, double alpha, double beta, bool maximizing_player);
