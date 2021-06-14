//
// Created by Alexander Mayorov on 13/06/2021.
//

#pragma once
#include <iomanip>

template<size_t N>
std::function<size_t (const std::array<std::array<int, N>, N>&)> Puzzle<N>::heuristic = &Puzzle::manhattanHeuristic;

template<size_t N>
Puzzle<N>::Puzzle(const Puzzle<N> &other) : tiles(other.tiles) {
   g = other.g;
   h = other.h;
   parent = other.parent;
}

template<size_t N>
Puzzle<N>::Puzzle(const std::array<std::array<int, N>, N> &tiles) : tiles(tiles) {
    g = 0;
    h = heuristic(tiles);
    final = hammingHeuristic(tiles) == 0;
}

template<size_t N>
Puzzle<N>::Puzzle(const std::array<std::array<int, N>, N> &tiles, const Puzzle<N> &parent) : tiles(tiles) {
    g = parent.g + 1;
    h = heuristic(tiles);
    final = hammingHeuristic(tiles) == 0;
    this->parent = &parent;
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
    return final;
}

template<size_t N>
const Puzzle<N>* Puzzle<N>::getParent() const {
    return parent;
}

template<size_t N>
void Puzzle<N>::setParent(const Puzzle<N> &newParent) {
    parent = &newParent;
    g = newParent.g + 1;
}

template<size_t N>
const std::array<std::array<int, N>, N>& Puzzle<N>::getTiles() const {
    return tiles;
}

template<size_t N>
bool Puzzle<N>::operator==(const Puzzle<N>& other) {
   return tiles == other.tiles;
}

template<size_t N>
static const std::array<std::array<int, N>, N>& swapLeft(const std::array<std::array<int, N>, N> &tiles, size_t row, size_t col) {
    auto *copy = new std::array<std::array<int, N>, N>(tiles);
    int tmp = tiles[row][col - 1];
    (*copy)[row][col - 1] = 0;
    (*copy)[row][col] = tmp;
    return *copy;
}

template<size_t N>
static const std::array<std::array<int, N>, N>& swapRight(const std::array<std::array<int, N>, N> &tiles, size_t row, size_t col) {
    auto copy = new std::array<std::array<int, N>, N>(tiles);
    int tmp = tiles[row][col + 1];
    (*copy)[row][col + 1] = 0;
    (*copy)[row][col] = tmp;
    return *copy;
}

template<size_t N>
static const std::array<std::array<int, N>, N>& swapTop(const std::array<std::array<int, N>, N> &tiles, size_t row, size_t col) {
    auto copy = new std::array<std::array<int, N>, N>(tiles);
    int tmp = tiles[row - 1][col];
    (*copy)[row - 1][col] = 0;
    (*copy)[row][col] = tmp;
    return *copy;
}

template<size_t N>
static const std::array<std::array<int, N>, N>& swapBottom(const std::array<std::array<int, N>, N> &tiles, size_t row, size_t col) {
    auto copy = new std::array<std::array<int, N>, N>(tiles);
    int tmp = tiles[row + 1][col];
    (*copy)[row + 1][col] = 0;
    (*copy)[row][col] = tmp;
    return *copy;
}

template<size_t N>
std::vector<Puzzle<N> *>* Puzzle<N>::getChildren() const {
    size_t i = 0;
    while (tiles[i / N][i % N] != 0) {
        ++i;
    }

    size_t row = i / N;
    size_t col = i % N;
    auto children = new std::vector<Puzzle<N> *>();

    if (col > 0)
        children->push_back(new Puzzle<N>(swapLeft(tiles, row, col), *this));
    if (col < N - 1)
        children->push_back(new Puzzle<N>(swapRight(tiles, row, col), *this));
    if (row > 0)
        children->push_back(new Puzzle<N>(swapTop(tiles, row, col), *this));
    if (row < N - 1)
        children->push_back(new Puzzle<N>(swapBottom(tiles, row, col), *this));

    return children;
}

template<size_t N>
size_t Puzzle<N>::hammingHeuristic(const std::array<std::array<int, N>, N> &tiles) {
    size_t h = 0;

    for (size_t i = 0; i < N * N; ++i) {
        int value = tiles[i / N][i % N];
        if (value != 0 && value != (int)(i + 1))
            ++h;
    }
    return h;
}

template<size_t N>
size_t Puzzle<N>::euclidianHeuristic(const std::array<std::array<int, N>, N> &tiles) {
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
size_t Puzzle<N>::manhattanHeuristic(const std::array<std::array<int, N>, N> &tiles) {
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
size_t Puzzle<N>::manhattanLinearConflictHeuristic(const std::array<std::array<int, N>, N> &tiles) {
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













