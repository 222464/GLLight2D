#include <Constructs/Matrix3x3i.h>

#include <iostream>

#include <assert.h>

Matrix3x3i::Matrix3x3i()
{
}

int &Matrix3x3i::operator[](int i)
{
	assert(i >= 0 && i < 9);

	return m_elements[i];
}

Matrix3x3i Matrix3x3i::operator*(const Matrix3x3i &other) const
{
	Matrix3x3i product;

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
		{
			int sum = 0;

			// jth row of this by ith column of other
			for(int d = 0; d < 3; d++)
				sum += Get(d, j) * other.Get(i, d);

			product.Set(i, j, sum);
		}

	return product;
}

Matrix3x3i Matrix3x3i::operator*=(const Matrix3x3i &other)
{
	return (*this) = (*this) * other;
}

bool Matrix3x3i::operator==(const Matrix3x3i &other) const
{
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 4; j++)
		{
			if(Get(i, j) != other.Get(i, j))
				return false;
		}

	return true;
}

bool Matrix3x3i::operator!=(const Matrix3x3i &other) const
{
	return !((*this) == other);
}

void Matrix3x3i::Set(int i, int j, int val)
{
	assert(i >= 0 && j >= 0 && i < 3 && j < 3);

	m_elements[3 * i + j] = val; // Row-major would be i + 4 * j
}

int Matrix3x3i::Get(int i, int j) const
{
	assert(i >= 0 && j >= 0 && i < 3 && j < 3);

	return m_elements[3 * i + j]; // Row-major would be i + 4 * j
}

void Matrix3x3i::SetIdentity()
{
	m_elements[0] = 1; m_elements[3] = 0; m_elements[6] = 0;
	m_elements[1] = 0; m_elements[4] = 1; m_elements[7] = 0;
	m_elements[2] = 0; m_elements[5] = 0; m_elements[8] = 1;
}

Matrix3x3i Matrix3x3i::Transpose() const
{
	Matrix3x3i transpose;

	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			transpose.Set(j, i, Get(i, j));

	return transpose;
}

int Matrix3x3i::Determinant() const
{
	return m_elements[0] * m_elements[4] * m_elements[8] +
		m_elements[3] * m_elements[7] * m_elements[2] +
		m_elements[6] * m_elements[1] * m_elements[5] -
		m_elements[2] * m_elements[4] * m_elements[6] - 
		m_elements[5] * m_elements[7] * m_elements[0] -
		m_elements[8] * m_elements[2] * m_elements[3];
}

Matrix3x3i Matrix3x3i::IdentityMatrix()
{
	Matrix3x3i identity;
	identity.SetIdentity();

	return identity;
}

Point3i Matrix3x3i::operator*(const Point3i &vec) const
{
	Point3i result;
	
	result.x = m_elements[0] * vec.x + m_elements[3] * vec.y + m_elements[6] * vec.z;
	result.y = m_elements[1] * vec.x + m_elements[4] * vec.y + m_elements[7] * vec.z;
	result.z = m_elements[2] * vec.x + m_elements[5] * vec.y + m_elements[8] * vec.z;

	return result;
}

Point2i Matrix3x3i::operator*(const Point2i &vec) const
{
	Point2i result;

	result.x = m_elements[0] * vec.x + m_elements[3] * vec.y + m_elements[6];
	result.y = m_elements[1] * vec.x + m_elements[4] * vec.y + m_elements[7];

	return result;
}

void Matrix3x3i::Print() const
{
	for(int j = 0; j < 3; j++)
	{
		for(int i = 0; i < 3; i++)
			std::cout << Get(i, j) << " ";

		std::cout << std::endl;
	}
}