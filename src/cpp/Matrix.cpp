#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <sstream>
#include "Matrix.hpp"
 
Matrix::Matrix(size_t width) : w(width), vals(width * width, 0) {}
 
Matrix::Matrix(const Matrix& other) : w(other.w), vals(other.vals) {}

Matrix::Matrix(const vector<int>& vals) : w(sqrt(vals.size())), vals(vals) {
	if (vals.size() % w != 0)
		throw new invalid_argument("`vals` should contain exactly (width * width) elements");
}

Matrix& Matrix::operator=(const Matrix& other)
{
	if (w != other.w)
		throw invalid_argument("Matrix dimensions doesn't match");
	vals = other.vals;
	return *this;
}

const int& Matrix::operator()(int i, int j) const
{
	return vals[i * w + j];
}

int& Matrix::operator()(int i, int j)
{
	return vals[i * w + j];
}

const int& Matrix::operator()(const pair<int, int>& coords) const
{
	return vals[coords.first * w + coords.second];
}

int& Matrix::operator()(const pair<int, int>& coords)
{
	return vals[coords.first * w + coords.second];
}

size_t Matrix::width() const
{
	return w;
}

string Matrix::toBytes() const
{
	ostringstream iss;

	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < w; ++j) {
			iss << vals[i * w + j];
		}
	}
	return iss.str();
}

void Matrix::swap(const pair<int, int>& p1, const pair<int, int>& p2)
{
	std::swap((*this)(p1), (*this)(p2));
}

pair<int, int> Matrix::find(int val) const
{
	for (int i = 0; i < w; ++i) {
		for (int j = 0; j < w; ++j) {
			if (vals[i * w + j] == val)
				return pair<int, int>(i, j);
		}
	}
	return pair<int, int>(-1, -1);
} 

ostream& operator<<(ostream& os, const Matrix& mat)
{
	int maxNum = mat.width() * mat.width() - 1;
	size_t fwidth = to_string(maxNum).length();

    for (size_t i = 0; i < mat.width(); ++i) {
        for (size_t j = 0; j < mat.width(); ++j) {
            os << setw(fwidth) << mat(i, j);
            if (j < mat.width() - 1) os << ' ';
        }
        if (i < mat.width() - 1) os << endl;
    }
    return os;
}
