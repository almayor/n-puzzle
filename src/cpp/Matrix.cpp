#include <iomanip>
#include <vector>
#include <string>
#include "Matrix.hpp"

 
Matrix::Matrix(size_t width) : w(width), vals(width, 0) {}
 
Matrix::Matrix(const vector<int>& vals) : w(sqrt(vals.size())), vals(vals) {
	if (vals.size() % w != 0)
		throw new invalid_argument("`vals` should contain exactly (width * width) elements");
}
 
Matrix::Matrix(const Matrix& other) : w(other.w), vals(other.vals) {}

Matrix& Matrix::operator=(const Matrix& other)
{
	if (w != other.w)
		throw invalid_argument("Matrix dimensions doesn't match");
	vals = other.vals;
	return *this;
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
	int tmp = (*this)(p1);
	(*this)(p1) = (*this)(p2);
	(*this)(p2) = tmp;
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
	size_t fwidth = to_string(mat.width() ^ 2 - 1).length();

    for (size_t i = 0; i < mat.width(); ++i) {
        for (size_t j = 0; j < mat.width(); ++j) {
        	if (j > 0) os << ' ';
            os << setw(fwidth) << mat(i, j);
        }
        os << endl;
    }
    return os;
}
