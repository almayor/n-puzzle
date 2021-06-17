//
// Created by Alexander Mayorov on 13/06/2021.
//

#pragma once
#include <iomanip>
using namespace std;

template<size_t N>
function<size_t (const matrix<N>&)> Puzzle<N>::heuristic = &Puzzle::manhattanHeuristic;

template<size_t N>
Puzzle<N>::Puzzle(const Puzzle<N> &other) : tiles(other.tiles) {
   g = other.g;
   h = other.h;
   parent = other.parent;
}

template<size_t N>
Puzzle<N>::Puzzle(unique_ptr<const matrix<N>> tiles) {
    g = 0;
    h = heuristic(tiles);
    this->tiles = tiles;
}

template<size_t N>
Puzzle<N>::Puzzle(unique_ptr<const matrix<N>> tiles, shared_ptr<const Puzzle> parent) : tiles(tiles) {
    g = parent.g + 1;
    h = heuristic(tiles);
    this->parent = parent;
}

template<size_t N>
Puzzle<N>::~Puzzle() {
    delete &tiles;
}

template<size_t N>
void Puzzle<N>::setHeuristic(HeuristicType type) {
    switch (type) {
        case HeuristicType::Hamming:
            Puzzle::heuristic = &Puzzle::hammingHeuristic;
            break;
        case HeuristicType::Euclidian:
            Puzzle::heuristic = &Puzzle::euclidianHeuristic;
            break;
        case HeuristicType::Manhattan:
            Puzzle::heuristic = &Puzzle::manhattanHeuristic;
            break;
        case HeuristicType::ManhattanAndLinearConflict:
            Puzzle::heuristic = &Puzzle::manhattanLinearConflictHeuristic;
            break;
    }
}

template<size_t N>
size_t Puzzle<N>::getDistance() const {
    return g + h;
}

template<size_t N>
bool Puzzle<N>::isFinal() const {
    return h == 0;
}

template<size_t N>
const Puzzle<N>& Puzzle<N>::getParent() const {
    return parent;
}

template<size_t N>
const matrix<N>& Puzzle<N>::getTiles() const {
    return tiles;
}

template<size_t N>
bool Puzzle<N>::operator==(const Puzzle<N>& other) {
   return tiles == other.tiles;
}

template<size_t N>
static unique_ptr<const matrix<N>> swapLeft(const matrix<N>& tiles, size_t row, size_t col) {
    auto copy = make_unique<matrix>(tiles);
    int tmp = tiles[row][col - 1];
    (*copy)[row][col - 1] = 0;
    (*copy)[row][col] = tmp;
    return copy;
}

template<size_t N>
static unique_ptr<const matrix<N>> swapRight(const matrix<N>& tiles, size_t row, size_t col) {
    auto copy = make_unique<matrix>(tiles);
    int tmp = tiles[row][col + 1];
    (*copy)[row][col + 1] = 0;
    (*copy)[row][col] = tmp;
    return copy;
}


template<size_t N>
static unique_ptr<const matrix<N>> swapTop(const matrix<N>& tiles, size_t row, size_t col) {
    auto copy = make_unique<matrix>(tiles);
    int tmp = tiles[row + 1][col];
    (*copy)[row + 1][col] = 0;
    (*copy)[row][col] = tmp;
    return copy;
}

template<size_t N>
static unique_ptr<matrix<N>> swapBottom(const matrix<N>& tiles, size_t row, size_t col) {
    auto copy = new std::array<std::array<int, N>, N>(tiles);
    int tmp = tiles[row + 1][col];
    (*copy)[row + 1][col] = 0;
    (*copy)[row][col] = tmp;
    return copy;
}

template<size_t N>
unique_ptr<vector<const Puzzle<N> *>> Puzzle<N>::getChildren() const {
    size_t i = 0;
    while (tiles[i / N][i % N] != 0) {
        ++i;
    }

    size_t row = i / N;
    size_t col = i % N;
    auto children = make_unique<vector<const Puzzle<N> *>>();

    if (col > 0) {
        unique_ptr<const matrix<N>> copy = swapLeft(tiles, row, col);
        children->push_back(new Puzzle<N>(copy, this->shared_from_this()));
    }
    if (col < N - 1)
        children->push_back(new Puzzle<N>(swapRight(tiles, row, col), this->shared_from_this()));
    if (row > 0)
        children->push_back(new Puzzle<N>(swapTop(tiles, row, col), this->shared_from_this()));
    if (row < N - 1)
        children->push_back(new Puzzle<N>(swapBottom(tiles, row, col), this->shared_from_this()));

    return children;
}

template<size_t N>
size_t Puzzle<N>::hammingHeuristic(const matrix<N>& tiles) {
    size_t h = 0;

    for (size_t i = 0; i < N * N; ++i) {
        int value = tiles[i / N][i % N];
        if (value != 0 && value != (int)(i + 1))
            ++h;
    }
    return h;
}

template<size_t N>
size_t Puzzle<N>::euclidianHeuristic(const matrix<N>& tiles) {
    size_t h = 0;

    for (size_t i = 0; i < N * N; ++i) {
        int value = tiles[i / N][i % N];
        if (value == 0) // empty tile doesn't contribute
            continue;

        int dstRow = (int)((value - 1) / N);
        int dstCol = (int)((value - 1) % N);
        int curRow = (int)(i / N);
        int curCol = (int)(i % N);
        h += floor(sqrt((dstRow - curRow) ^ 2 + (dstCol - curCol) ^ 2));
    }
    return h;
}

template<size_t N>
size_t Puzzle<N>::manhattanHeuristic(const matrix<N>& tiles) {
    size_t h = 0;

    for (size_t i = 0; i < N * N; ++i) {
        int value = tiles[i / N][i % N];
        if (value > 0) {
            int dstRow = (int)((value - 1) / N);
            int dstCol = (int)((value - 1) % N);
            int curRow = (int)(i / N);
            int curCol = (int)(i % N);
            h += abs(dstRow - curRow) + abs(dstCol - curCol);
        }
    }
    return h;
}

template<size_t N>
size_t Puzzle<N>::manhattanLinearConflictHeuristic(const matrix<N>& tiles) {
    // TODO
    return manhattanHeuristic(tiles);
}

template<size_t N>
std::ostream& operator<<(std::ostream& os, const Puzzle<N>& puzzle) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            os << std::setw(2) << puzzle.getTiles()[i][j] << " ";
        }
        os << std::endl;
    }
    return os;
}













