#pragma once
#include <iostream>
#include <array>
#include <map>
#include <queue>
#include <stack>
#include "Puzzle.hpp"
using namespace std;

template<size_t N>
static void tracePath(const Puzzle<N> *p) {
    stack<const Puzzle<N> *> path;
    while (p != nullptr) {
        path.push(p);
        p = p->getParent();
    }
    cout << "Path length = " << path.size() << endl;
    while (!path.empty()) {
        const Puzzle<N> *q = path.top();
        path.pop();
        cout << *q << endl;
    }
}
//
//template<size_t N>
//void aStar(const array<array<int, N>, N> *tiles) {
//    map<Puzzle<N> *, size_t, typename Puzzle<N>::TileComparator> closed;
//    priority_queue<Puzzle<N> *, vector<Puzzle<N> *>, typename Puzzle<N>::DistComparator> opened;
//    auto *node = new Puzzle<N>(*tiles);
//
//    bool finished = false;
//    opened.push(node);
//    while (!opened.empty() && !finished) {
//        cout << opened.size() << endl;
//        node = opened.top();
//        if (node->isFinal())
//            finished = true;
//
//        opened.pop();
//        for (Puzzle<N> *child : *(node->getChildren())) {
//            auto lookup = closed.find(child);
//            if (lookup == closed.end() || lookup->second > child->getDistance()) {
//                opened.push(child);
//            } else {
//                delete child;
//            }
//        }
//        closed[node] = node->getDistance();
//    }
//
//    if (finished == 0) {
//        cout << "FAILED" << endl;
//        return ;
//    }
//    cout << "Considered states = " << closed.size() + opened.size() << endl;
//    tracePath(node);
//}

template<size_t N>
void aStar(const array<array<int, N>, N> *tiles) {
    auto *node = new Puzzle<N>(*tiles);
    cout << *node << endl;
    for (Puzzle<N> *child : *(node->getChildren())) {
        cout << *child << endl;
    }
    cout << *node << endl;
}

