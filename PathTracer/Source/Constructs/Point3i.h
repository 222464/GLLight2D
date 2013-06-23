#pragma once

class Point3i
{
public:
	int x, y, z;

	Point3i(int X, int Y, int Z);
	Point3i();

	Point3i operator*(int scale) const;
	Point3i operator/(int scale) const;
	Point3i operator+(const Point3i &other) const;
	Point3i operator-(const Point3i &other) const;
	Point3i operator*(const Point3i &other) const;
	Point3i operator/(const Point3i &other) const;
	Point3i operator-() const;

	const Point3i &operator*=(int scale);
	const Point3i &operator/=(int scale);
	const Point3i &operator+=(const Point3i &other);
	const Point3i &operator-=(const Point3i &other);
	const Point3i &operator*=(const Point3i &other);
	const Point3i &operator/=(const Point3i &other);
	
	bool operator==(const Point3i &other) const;
	bool operator!=(const Point3i &other) const;
};