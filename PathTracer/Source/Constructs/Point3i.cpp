#include <Constructs/Point3i.h>

Point3i::Point3i()
{
}

Point3i::Point3i(int X, int Y, int Z)
	: x(X), y(Y), z(Z)
{
}


Point3i Point3i::operator*(int scale) const 
{
	return Point3i(x * scale, y * scale, z * scale);
}

Point3i Point3i::operator/(int scale) const 
{
	return Point3i(x / scale, y / scale, z / scale);
}

Point3i Point3i::operator+(const Point3i &other) const 
{
	return Point3i(x + other.x, y + other.y, z + other.z);
}

Point3i Point3i::operator-(const Point3i &other) const 
{
	return Point3i(x - other.x, y - other.y, z - other.z);
}

Point3i Point3i::operator*(const Point3i &other) const 
{
	return Point3i(x * other.x, y * other.y, z * other.z);
}

Point3i Point3i::operator/(const Point3i &other) const 
{
	return Point3i(x / other.x, y / other.y, z / other.z);
}

Point3i Point3i::operator-() const
{
	return Point3i(-x, -y, -z);
}

const Point3i &Point3i::operator*=(int scale) 
{
	x *= scale;
	y *= scale;
	z *= scale;

	return *this;
}

const Point3i &Point3i::operator/=(int scale) 
{
	x /= scale;
	y /= scale;
	z /= scale;

	return *this;
}

const Point3i &Point3i::operator+=(const Point3i &other) 
{
	x += other.x;
	y += other.y;
	z += other.z;

	return *this;
}

const Point3i &Point3i::operator-=(const Point3i &other) 
{
	x -= other.x;
	y -= other.y;
	z -= other.z;

	return *this;
}

const Point3i &Point3i::operator*=(const Point3i &other) 
{
	x *= other.x;
	y *= other.y;
	z *= other.z;

	return *this;
}

const Point3i &Point3i::operator/=(const Point3i &other) 
{
	x /= other.x;
	y /= other.y;
	z /= other.z;

	return *this;
}

bool Point3i::operator==(const Point3i &other) const
{
	if(x == other.x && y == other.y && z == other.z)
		return true;

	return false;
}

bool Point3i::operator!=(const Point3i &other) const
{
	if(x != other.x || y != other.y || z != other.z)
		return true;

	return false;
}