#include <iostream>
#include <array>
#include <map>
#include <queue>
#include <stack>
#include "Puzzle.hpp"
#include "aStar.tpp"
using namespace std;


int main() {
    Puzzle<3>::setHeuristic(Puzzle<3>::HeuristicType::Manhattan);

    auto *tiles = new array<array<int, 3>, 3>({{
        {5, 8, 2},
        {1, 3, 6},
        {7, 4, 0}
    }});
    aStar<3>(tiles);
    return 0;
}
