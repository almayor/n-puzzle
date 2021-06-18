#include <iostream>
#include <array>
#include <unordered_map>
#include <queue>
#include <stack>
#include "Puzzle.hpp"
using namespace std;

static stack<shared_ptr<const Puzzle>> tracePath(shared_ptr<const Puzzle> puzzle)
{
	stack<shared_ptr<const Puzzle>> path;
	while (puzzle != nullptr) {
		path.push(puzzle);
		puzzle = puzzle->getParent();
	}
	return path;
}

tuple<stack<shared_ptr<const Puzzle>>, size_t, size_t> aStar(shared_ptr<const Puzzle> node)
{
   unordered_map<string, size_t> closed;
   priority_queue<shared_ptr<const Puzzle>,
				  vector<shared_ptr<const Puzzle>>,
				  typename Puzzle::DistComparator> opened;

	closed[node->getBytes()] = node->fDist();
	opened.push(node);
	size_t nIter = 0;
	size_t maxSize = 0;

	while (!opened.empty()) {
	    node = opened.top();
	    opened.pop();
	   
	    if (node->isFinal())
		   break;

	    for (shared_ptr<const Puzzle> child : node->getChildren()) {
		    auto lookup = closed.find(child->getBytes());
		    if (lookup != closed.end() && lookup->second <= child->fDist())
				continue;
			opened.push(child);
			closed[child->getBytes()] = child->fDist();
			maxSize = max(maxSize, opened.size());
	    }
		++nIter;
    }
	stack<shared_ptr<const Puzzle>> path = tracePath(node);
	return make_tuple(path, nIter, maxSize);
}
