#pragma once
#include <vector>
#include <string>
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
	virtual ~Matrix() = default;
	Matrix& operator=(const Matrix& other);

	int& operator()(int i, int j);
	int& operator()(const pair<int, int>& coords);
	const int& operator()(int i, int j) const;
	const int& operator()(const pair<int, int>& coords) const;
	
	size_t width() const;
	string toBytes() const;
	pair<int, int> find(int val) const;
	void swap(const pair<int, int>& p1, const pair<int, int>& p2);
};

ostream& operator<<(ostream& os, const Matrix& puzzle);
