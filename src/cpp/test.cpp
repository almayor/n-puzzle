#include <iostream>
#include <stack>
#include <stdlib.h>
#include "Matrix.hpp"
#include "Puzzle.hpp"

tuple<stack<shared_ptr<const Puzzle>>, size_t, size_t> aStar(shared_ptr<const Puzzle> node);

int main() {
	Matrix goal = {{
		1, 2, 3,
		8, 0, 4,
		7, 6, 5
	}};
	Puzzle::setGoal(goal);
	Puzzle::setHeuristic(Puzzle::HeuristicType::ManhattanLinConf);

	Matrix start = {{
		2, 3, 4,
		1, 8, 5,
		0, 7, 6
	}};
	auto puzzle = make_shared<const Puzzle>(start);
	
	if (!puzzle->isSolvable()) {
		cout << "Puzzle is unsolvable" << endl;
		exit(EXIT_FAILURE);
	}

	stack<shared_ptr<const Puzzle>> path;
	size_t nIter, maxSize, pathLen;
	tie(path, nIter, maxSize) = aStar(puzzle);
	pathLen = path.size();

	while (!path.empty()) {
		cout << *path.top() << endl << endl;
		path.pop();
	}

	cout << "PATH LENGTH = " << pathLen << endl;
}