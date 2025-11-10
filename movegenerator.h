#pragma once
#include "GameState.h"
#include "Move.h"
#include <vector>

class MoveGenerator {
private:
    static bool squareIsAttacked(int square, const GameState& state, bool byWhite);
    static void generatePawnMoves(int square, const GameState& state, std::vector<Move>& moves);
    static void generateKnightMoves(int square, const GameState& state, std::vector<Move>& moves);
    static void generateKingMoves(int square, const GameState& state, std::vector<Move>& moves);
    static void generateSlidingMoves(int square, const GameState& state, std::vector<Move>& moves, const std::vector<int>& directions);
};