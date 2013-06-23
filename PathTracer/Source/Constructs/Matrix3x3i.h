#pragma once

#include <Constructs/Point3i.h>
#include <Constructs/Point2i.h>
#include <vector>

/*
	Different from mathematics standard:

	i means column
	j means row
*/

// 1D array stores the matrix in column-major order, like OpenGL

class Matrix3x3i
{
public:
	int m_elements[9];

	Matrix3x3i();
	Matrix3x3i(std::vector<int> sourceArray);

	int &operator[](int i);

	Matrix3x3i operator*(const Matrix3x3i &other) const;
	Matrix3x3i operator*=(const Matrix3x3i &other);

	bool operator==(const Matrix3x3i &other) const;
	bool operator!=(const Matrix3x3i &other) const;

	void Set(int i, int j, int val);
	int Get(int i, int j) const;

	void SetIdentity();
	Matrix3x3i Transpose() const;

	int Determinant() const;

	// Transformation matrix generators
	static Matrix3x3i IdentityMatrix();

	Point3i operator*(const Point3i &vec) const;
	Point2i operator*(const Point2i &vec) const;

	void Print() const;
};