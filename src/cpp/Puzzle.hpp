//
// Created by Alexander Mayorov on 13/06/2021.
//

#pragma once
#include <array>

template<size_t N>
class Puzzle {
private:
    const Puzzle* parent = nullptr;
    std::array<std::array<int, N>, N> const& tiles;

    size_t g;
    size_t h;
    bool final = false;

    static size_t hammingHeuristic(std::array<std::array<int, N>, N> const& tiles);
    static size_t euclidianHeuristic(std::array<std::array<int, N>, N> const& tiles);
    static size_t manhattanHeuristic(std::array<std::array<int, N>, N> const& tiles);
    static size_t manhattanLinearConflictHeuristic(std::array<std::array<int, N>, N> const& tiles);

    static std::function<size_t (std::array<std::array<int, N>, N> const&)> heuristic;

public:
    enum class HeuristicType {
        Hamming,
        Euclidian,
        Manhattan,
        ManhattanAndLinearConflict,
    };
    static void setHeuristic(HeuristicType type);

    Puzzle() = delete;
    Puzzle(const Puzzle<N> &other);
    explicit Puzzle(std::array<std::array<int, N>, N> const& tiles);
    explicit Puzzle(std::array<std::array<int, N>, N> const& tiles, Puzzle<N> const& parent);
    virtual ~Puzzle();

    bool operator==(Puzzle<N> const& other);

    size_t getDistance() const;
    std::array<std::array<int, N>, N> const& getTiles() const;
    std::vector<Puzzle<N> *>* getChildren() const;
    Puzzle<N> const* getParent() const;

    bool isFinal() const;
    void setParent(Puzzle<N> const& parent);

    struct TileComparator {
        bool operator()(const Puzzle<N> *p1, const Puzzle<N> *p2) const {
            return p1->getTiles() > p2->getTiles();
        }
    };
    struct DistComparator {
        bool operator()(const Puzzle<N> *p1, const Puzzle<N> *p2) const {
            return p1->getDistance() > p2->getDistance();
        }
    };
};

template<size_t N>
std::ostream& operator<<(std::ostream& os, Puzzle<N> const& puzzle);

#include "Puzzle.tpp"
