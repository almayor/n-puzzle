#include <iostream>
#include <vector>
#include <array>
#include <string>
#include "Matrix.hpp"
#include "Puzzle.hpp"

int main() {
	Matrix goal = {{
		1, 2, 3,
		8, 0, 4,
		7, 6, 5
	}};
	Puzzle::setGoal(goal);
	Puzzle::setHeuristic(Puzzle::HeuristicType::Manhattan);

	Puzzle puzzle({{
		1, 2, 3,
		5, 8, 4,
		7, 6, 0
	}});
	cout << puzzle;
	cout << puzzle.hDist() << endl;
	// Puzzle<3>::setGoal(mat);
	// cout << mat << endl;
	// Puzzle<3> = 
	// cout << mat << endl;
	// Puzzle<3> puzzle({{0, 1, 2}});
	// cout << puzzle << endl;
	// // vector<shared_ptr<const Puzzle<3>>> children = puzzle.getChildren();
	// for (auto child : puzzle.getChildren()) {
	// 	cout << *child << endl;
	// }
	// cout << puzzle << endl;
}