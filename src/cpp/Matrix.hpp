#pragma once
#include <array>
#include <string>
#include <sstream>
using namespace std;

class Matrix
{
private:
	const size_t w;
	vector<int> vals;

public:
	Matrix() = delete;
	Matrix(size_t width);
	Matrix(const vector<int>& vals);
	Matrix(const Matrix& other);
	~Matrix() = default;
	Matrix& operator=(const Matrix& other);

	inline const int& operator()(int i, int j) const {
		return vals[i * w + j];
	}
	inline int& operator()(int i, int j) {
		return vals[i * w + j];
	}
	inline const int& operator()(const pair<int, int>& coords) const {
		return vals[coords.first * w + coords.second];
	}
	inline int& operator()(const pair<int, int>& coords) {
		return vals[coords.first * w + coords.second];
	}
	inline size_t width() const {
		return w;
	}

	string toBytes() const;
	pair<int, int> find(int val) const;
	void swap(const pair<int, int>& p1, const pair<int, int>& p2);
};

ostream& operator<<(std::ostream& os, const Matrix& puzzle);
