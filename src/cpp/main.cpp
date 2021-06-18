#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <stdlib.h>

#include "Matrix.hpp"
#include "Puzzle.hpp"

tuple<stack<shared_ptr<const Puzzle>>, size_t, size_t> aStar(shared_ptr<const Puzzle> node);

static Matrix parse_input(istream& is)
{
	string line;
	size_t width;
	do { getline(is, line); } while (line[0] == '#');
	stringstream ss(line);
	ss >> width;

	Matrix mat(width);
	for (int i = 0; i < width; ++i) {
		for (int j = 0; j < width; ++j) {
			is >> mat(i, j);
		}
	}
	return mat;
}

static void fillTopRight(Matrix& mat, int val,
						const pair<int, int> start,
						const pair<int, int> end);

static void fillBottomLeft(Matrix& mat, int val,
						const pair<int, int> start,
						const pair<int, int> end)
{
	int i = end.first;
	for (int j = end.second; j > start.second; --j) {
		mat(i, j) = val++;
	}
	int j = start.second;
	for (int i = end.first; i > start.first; --i) {
		mat(i, j) = val++;
	}

	if (val < mat.width() * mat.width())
		mat(start.first, start.second) = val++;
	if (end.second > start.second) {
		fillTopRight(
			mat, val,
			{start.first, start.second + 1},
			{end.first - 1, end.second}
		);
	}
}

static void fillTopRight(Matrix& mat, int val,
						const pair<int, int> start,
						const pair<int, int> end)
{
	int i = start.first;
	for (int j = start.second; j < end.second; ++j) {
		mat(i, j) = val++;
	}
	int j = end.second;
	for (int i = start.first; i < end.first; ++i) {
		mat(i, j) = val++;
	}

	if (val < mat.width() * mat.width())
		mat(end.first, end.second) = val++;
	if (end.first > start.first) {
		fillBottomLeft(
			mat, val,
			{start.first + 1, start.second},
			{end.first, end.second - 1}
		);
	}
}

static Matrix make_goal(size_t width)
{
	Matrix goal(width);
	fillTopRight(goal, 1, {0, 0}, {width - 1, width - 1});
	return goal;
}

int main()
{
	Matrix start = parse_input(cin);
	Matrix end = make_goal(start.width());

	Puzzle::setGoal(end);
	Puzzle::setHeuristic(Puzzle::HeuristicType::ManhattanLinConf);
	auto puzzle = make_shared<const Puzzle>(start);

	if (!puzzle->isSolvable()) {
		cerr << "Puzzle is unsolvable" << endl;
	 	exit(EXIT_FAILURE);
	}

	stack<shared_ptr<const Puzzle>> path;
	size_t nIter, maxQueueSize, pathLen;
	tie(path, nIter, maxQueueSize) = aStar(puzzle);
	pathLen = path.size() - 1;

	while (!path.empty()) {
		cout << *path.top() << endl << endl;
		path.pop();
	}

	cout << "total moves:\t\t\t" << pathLen << endl;
	cout << "time complexity:\t\t" << nIter << endl;
	cout << "space complexity:\t\t" << maxQueueSize << endl;
}