//
// Created by Alexander Mayorov on 13/06/2021.
//

#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <ostream>
#include "Matrix.hpp"

using namespace std;

class Puzzle : public enable_shared_from_this<Puzzle>
{

private:
    const size_t w;

    shared_ptr<const Puzzle> parent = nullptr;
    Matrix tiles;
    string bytes;
    pair<int, int> zero_loc;

    size_t f = 0;
    size_t g = 0;
    size_t h = 0;

    size_t countLinearConflicts(int idx, bool column) const;

    typedef size_t (Puzzle::*heuristicFn)(bool);
    static heuristicFn heuristic;
    static unordered_map<int, pair<int, int>> goal;

public:

    Puzzle() = delete;
    Puzzle(const Matrix& tiles);
    explicit Puzzle(const Puzzle &other);
    explicit Puzzle(const Matrix& tiles,
                    const pair<int, int>& zero_loc,
                    shared_ptr<const Puzzle> parent);

    virtual ~Puzzle() = default;
    bool operator==(const Puzzle& other) const;
    Puzzle& operator=(const Puzzle& other);

    const Matrix& getTiles() const;
    shared_ptr<const Puzzle> getParent() const;
    vector<shared_ptr<const Puzzle>> getChildren() const;
    bool isSolvable() const;

    inline const string getBytes() const { return bytes; }
    inline size_t fDist() const { return f; }
    inline size_t gDist() const { return g; }
    inline size_t hDist() const { return h; }
    inline size_t width() const { return w; }
    inline bool isFinal() const { return h == 0; }

    struct Hasher
    {
        size_t operator()(const Puzzle& puzzle) const;
    };

    enum class HeuristicType
    {
        Hamming,
        Euclidian,
        Manhattan,
        ManhattanLinConf,
    };

    static void setHeuristic(HeuristicType type);
    static void setGoal(const Matrix& mat);

    size_t manhattanHeuristic(bool relativeToParent = false);
    size_t euclidianHeuristic(bool relativeToParent = false);
    size_t hammingHeuristic(bool relativeToParent = false);
    size_t linConfHeuristic(bool relativeToParent = false);
    size_t manhattanLinConfHeuristic(bool relativeToParent = false);
};

ostream& operator<<(ostream& os, const Puzzle& puzzle);
