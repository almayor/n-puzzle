#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <cstdlib>

#include "Matrix.hpp"
#include "Puzzle.hpp"

tuple<stack<shared_ptr<const Puzzle>>, size_t, size_t> aStar(shared_ptr<const Puzzle> node);

static void printHelp(const string& programName)
{
	cout << "usage: " << programName << " [-h]"
		 << " [-r {manhattan, hamming, euclidian, manhattan_linconf}]"
		 << endl << endl
		 << "optional arguments:" << endl
		 << "  -h, --help" << "\t\tshow this help message and exit" << endl
		 << "  -r" << "\t\t\ttype of heuristic (manhattan_linconf by default)" << endl;
}

static Puzzle::HeuristicType parseArgs(int argc, const char *argv[])
{
	vector<string> args(argv, argv + argc);
	Puzzle::HeuristicType hType = Puzzle::HeuristicType::ManhattanLinConf;
	int i = 1;

	while (i < argc) {
		if (args[i] == "-h" || args[i] == "--help") {
			printHelp(args[0]);
			exit(EXIT_SUCCESS);
		}
		if (args[i] == "-r")
		{
			++i;
			if (i == argc) {
				printHelp(args[0]);
				throw invalid_argument("\nPlease specify heuristic name after `-r`");
			}
			if (args[i] == "manhattan") {
				hType = Puzzle::HeuristicType::Manhattan;
			} else if (args[i] == "hamming") {
				hType = Puzzle::HeuristicType::Hamming;
			} else if (args[i] == "euclidian") {
				hType = Puzzle::HeuristicType::Euclidian;
			} else if (args[i] == "manhattan_linconf") {
				hType = Puzzle::HeuristicType::ManhattanLinConf;
			} else {
				throw invalid_argument("Unknown heuristic name");
			}
		}
		++i;
	}
	return hType;
}

static Matrix parseInput(istream& is)
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

static void fillTopRight(Matrix& mat, int val, pair<int, int> start, pair<int, int> end);

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

static Matrix makeGoal(size_t width)
{
	Matrix goal(width);
	fillTopRight(goal, 1, {0, 0}, {width - 1, width - 1});
	return goal;
}

int main(int argc, const char *argv[])
{
	shared_ptr<const Puzzle> puzzle;

	try {
		Puzzle::HeuristicType hType = parseArgs(argc, argv);
		Matrix start = parseInput(cin);
		Matrix end = makeGoal(start.width());
		Puzzle::setGoal(end);
		Puzzle::setHeuristic(hType);
		puzzle = make_shared<const Puzzle>(start);
	} catch (const invalid_argument& e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}

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