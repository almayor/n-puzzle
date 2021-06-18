//
// Created by Alexander Mayorov on 13/06/2021.
//

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include "Puzzle.hpp"
#include "Matrix.hpp"

using namespace std;

Puzzle::Puzzle(const Puzzle& other) : tiles(other.tiles), w(other.width())
{
    bytes = other.bytes;
    zero_loc = other.zero_loc;
    parent = other.parent;

    g = other.g;
    h = other.h;
    f = other.f;
}

Puzzle::Puzzle(const Matrix& tiles) : tiles(tiles), w(tiles.width())
{
    bytes = tiles.toBytes();
    zero_loc = tiles.find(0);

    g = 0;
    h = invoke(Puzzle::heuristic, *this, false);
    f = g + h;
}

Puzzle::Puzzle(const Matrix& tiles,
               const pair<int, int>& zero_loc,
               shared_ptr<const Puzzle> parent)
:
    tiles(tiles),
    w(tiles.width()),
    zero_loc(zero_loc)
{
    bytes = tiles.toBytes();
    this->parent = parent;
    g = parent->g + 1;
    h = invoke(Puzzle::heuristic, *this, false);
    f = g + h;
}

bool Puzzle::operator==(const Puzzle& other) const
{
    return bytes == other.bytes;
}

const Matrix& Puzzle::getTiles() const
{
    return tiles;
}

shared_ptr<const Puzzle> Puzzle::getParent() const
{
    return parent;
}

static shared_ptr<const Puzzle> getChild(shared_ptr<const Puzzle> parent,
                                         pair<int, int> p1,
                                         pair<int, int> p2)
{
    Matrix copy = parent->getTiles();
    copy.swap(p1, p2);
    auto child = make_shared<const Puzzle>(copy, p2, parent);
    return child;
}

vector<shared_ptr<const Puzzle>> Puzzle::getChildren() const
{
    vector<shared_ptr<const Puzzle>> children;
    int i, j, k, l;
    tie(i, j) = zero_loc;
    tie(k, l) = parent ? parent->zero_loc : pair<int, int>({INT_MIN, INT_MIN});

    if (i > 0 && i - 1 != k)
        children.push_back(getChild(shared_from_this(), {i, j}, {i - 1, j}));
    if (j > 0 && j - 1 != l)
        children.push_back(getChild(shared_from_this(), {i, j}, {i, j - 1}));
    if (i < w - 1 && i + 1 != k)
        children.push_back(getChild(shared_from_this(), {i, j}, {i + 1, j}));
    if (j < w - 1 && j + 1 != l)
        children.push_back(getChild(shared_from_this(), {i, j}, {i, j + 1}));

    return children;
}

bool Puzzle::isSolvable() const
{
    size_t nPerm = 0;

    for (int idx1 = 0; idx1 < w * w; ++idx1) {
        int val1 = tiles(idx1 / w, idx1 % w);
        for (int idx2 = idx1 + 1; idx2 < w * w; ++idx2) {
            int val2 = tiles(idx2 / w, idx2 % w);
            if (val1 != 0 && val2 != 0 && goal[val1] > goal[val2])
                ++nPerm;
        }
    }
    if (w % 2 == 1 && nPerm % 2 == 1)
        return false;
    if (w % 2 == 0 && (nPerm + zero_loc.first) % 2 != w / 2 % 2)
        return false;
    return true;
}

size_t Puzzle::manhattanLinConfHeuristic(bool relativeToParent)
{
    if (parent == nullptr) {
        return manhattanHeuristic() + linConfHeuristic();
    }

    size_t hRel = manhattanHeuristic(true) + linConfHeuristic(true);
    return relativeToParent ? hRel : hRel + parent->h;
}

size_t Puzzle::manhattanHeuristic(bool relativeToParent)
{
    auto singleHeuristic = [](const Matrix& tiles, pair<int, int> coords) -> size_t
    {
        int val = tiles(coords);
        if (val == 0) return 0;
        int i, j, iDst, jDst;
        tie(i, j) = coords;
        tie(iDst, jDst) = goal[val];
        return abs(i - iDst) + abs(j - jDst);
    };

    if (parent == nullptr) {
        size_t h = 0;
        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < w; ++j) {
                h += singleHeuristic(tiles, {i, j});
            }
        }
        return h;
    }

    size_t hRel = singleHeuristic(tiles, parent->zero_loc)
                - singleHeuristic(parent->tiles, zero_loc);
    
    return relativeToParent ? hRel : hRel + parent->h;
}

size_t Puzzle::euclidianHeuristic(bool relativeToParent)
{
    auto singleHeuristic = [](const Matrix& tiles, pair<int, int> coords) -> size_t
    {
        int val = tiles(coords);
        if (val == 0) return 0;
        int i, j, iDst, jDst;
        tie(i, j) = coords;
        tie(iDst, jDst) = goal[val];
        return sqrt((i - iDst) ^ 2 + (j - jDst) ^ 2);
    };

    if (parent == nullptr) {
        size_t h = 0;
        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < w; ++j) {
                h += singleHeuristic(tiles, {i, j});
            }
        }
        return h;
    }

    size_t hRel = singleHeuristic(tiles, parent->zero_loc)
                - singleHeuristic(parent->tiles, zero_loc);
    
    return relativeToParent ? hRel : hRel + parent->h;
}

size_t Puzzle::hammingHeuristic(bool relativeToParent)
{
    auto singleHeuristic = [](const Matrix& tiles, pair<int, int> coords) -> size_t
    {
        int val = tiles(coords);
        if (val == 0) return 0;
        return Puzzle::goal[val] == coords ? 0 : 1;
    };

    if (parent == nullptr) {
        size_t h = 0;
        for (int i = 0; i < w; ++i) {
            for (int j = 0; j < w; ++j) {
                h += singleHeuristic(tiles, {i, j});
            }
        }
        return h;
    }

    size_t hRel = singleHeuristic(tiles, parent->zero_loc)
                - singleHeuristic(parent->tiles, zero_loc);
    
    return relativeToParent ? hRel : hRel + parent->h;
}

size_t Puzzle::linConfHeuristic(bool relativeToParent)
{
    if (parent == nullptr) {
        size_t h = 0;
        for (int i = 0; i < w; ++i) {
            h += 2 * countLinearConflictsInRow(i);
        }
        for (int j = 0; j < w; ++j) {
            h += 2 * countLinearConflictsInColumn(j);
        }
        return h;
    }

    int i, j, k, l;
    tie(i, j) = zero_loc;
    tie(k, l) = parent->zero_loc;
    size_t hRel = 0;

    if (i == k) {
        hRel = 2 * (
            countLinearConflictsInRow(l) -
            parent->countLinearConflictsInRow(j)
        );
    } else {
        hRel = 2 * (
            countLinearConflictsInColumn(k) -
            parent->countLinearConflictsInColumn(i)
        );
    }

    return relativeToParent ? hRel : hRel + parent->h;
}

static int maxDegreeVertexInConflictGraph(
    const unordered_map<int, unordered_set<int>> conflictGraph)
{
    size_t max_size = 0;
    int max_val = -1;

    for (auto& it : conflictGraph) {
        if (it.second.size() > max_size) {
            max_size = it.second.size();
            max_val = it.first;
        }
    }
    return max_val;
}

static size_t countConflicts(size_t nPairs, unordered_map<int, unordered_set<int>>& conflictGraph)
{
    size_t nConf = 0;

    while (nPairs > 0) {
        int node = maxDegreeVertexInConflictGraph(conflictGraph);
        unordered_set<int> neighbors = conflictGraph[node];
        nPairs -= neighbors.size();
        for (int neighbor : neighbors) {
            ++nConf;
            conflictGraph[neighbor].erase(node);
        }
        conflictGraph.erase(node);
    }
    return nConf;
}

size_t Puzzle::countLinearConflictsInRow(int idx) const
{
    size_t nPairs = 0;
    unordered_map<int, unordered_set<int>> conflictGraph;

    for (int j = 0; j < w; ++j) {
        int v1 = tiles(idx, j);
        pair<int, int> t1 = goal[v1];
        if (v1 == 0 || t1.first != idx)
            continue;

        for (int k = j + 1; k < w; ++k) {
            int v2 = tiles(idx, k);
            pair<int, int> t2 = goal[v2];
            if (v2 == 0 || t2.first != idx)
                continue;

            if (t1.second > t2.second) {
                if (conflictGraph.find(v1) == conflictGraph.end())
                    conflictGraph[v1] = unordered_set<int>();
                if (conflictGraph.find(v2) == conflictGraph.end())
                    conflictGraph[v2] = unordered_set<int>();
                conflictGraph[v1].insert(v2);
                conflictGraph[v2].insert(v1);
                ++nPairs;
            }
        }
    }
    return countConflicts(nPairs, conflictGraph);
}

size_t Puzzle::countLinearConflictsInColumn(int idx) const
{
    size_t nPairs = 0;
    unordered_map<int, unordered_set<int>> conflictGraph;

    for (int j = 0; j < w; ++j) {
        int v1 = tiles(j, idx);
        pair<int, int> t1 = goal[v1];
        if (v1 == 0 || t1.second != idx)
            continue;
        for (int k = j + 1; k < w; ++k) {
            int v2 = tiles(k, idx);
            pair<int, int> t2 = goal[v2];
            if (v2 == 0 || t2.second != idx)
                continue;
            if (t1.first > t2.first) {
                if (conflictGraph.find(v1) == conflictGraph.end())
                    conflictGraph[v1] = unordered_set<int>();
                if (conflictGraph.find(v2) == conflictGraph.end())
                    conflictGraph[v2] = unordered_set<int>();
                conflictGraph[v1].insert(v2);
                conflictGraph[v2].insert(v1);
                ++nPairs;
            }
        }
    }
    return countConflicts(nPairs, conflictGraph);
}

bool Puzzle::DistComparator::operator()(shared_ptr<const Puzzle> p1, shared_ptr<const Puzzle> p2) const
{
    if (p1->fDist() > p2->fDist())
        return true;
    if (p1->gDist() < p2->gDist())
        return true;
    return false;
}

void Puzzle::setGoal(const Matrix& mat)
{
    Puzzle::goal.clear();
    for (int i = 0; i < mat.width(); ++i) {
        for (int j = 0; j < mat.width(); ++j) {
            int val = mat(i, j);
            Puzzle::goal[val] = {i, j};
        }
    }
}

void Puzzle::setHeuristic(Puzzle::HeuristicType type)
{
    switch (type)
    {
        case Puzzle::HeuristicType::Hamming:
            Puzzle::heuristic = &Puzzle::hammingHeuristic;
            break;
        case Puzzle::HeuristicType::Euclidian:
            Puzzle::heuristic = &Puzzle::euclidianHeuristic;
            break;
        case Puzzle::HeuristicType::Manhattan:
            Puzzle::heuristic = &Puzzle::manhattanHeuristic;
            break;
        case Puzzle::HeuristicType::ManhattanLinConf:
            Puzzle::heuristic = &Puzzle::manhattanLinConfHeuristic;
            break;
    }
}

typedef size_t (Puzzle::*heuristicFn)(bool);
heuristicFn Puzzle::heuristic = &Puzzle::manhattanHeuristic;
unordered_map<int, pair<int, int>> Puzzle::goal = {};

ostream& operator<<(std::ostream& os, const Puzzle& puzzle)
{
    os << puzzle.getTiles();
    return os;
}
