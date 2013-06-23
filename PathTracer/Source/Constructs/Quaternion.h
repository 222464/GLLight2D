#pragma once

#include <Constructs/Vec3f.h>
#include <Constructs/Vec4f.h>
#include <Constructs/Matrix3x3f.h>
#include <Constructs/Matrix4x4f.h>

class Quaternion
{
public:
	static const float s_quaternionNormalizationTolerance;
	static const float s_quaternionDotTolerance;

	float w, x, y, z;

	Quaternion(); // Defaults to multiplicative identity
	Quaternion(float W, float X, float Y, float Z);
	Quaternion(float angle, const Vec3f &axis);
	Quaternion(const Vec3f &eulerAngles);
	Quaternion(const Matrix3x3f &mat);

	float Magnitude() const;
	float MagnitudeSquared() const;
	Quaternion Normalize() const;
	void NormalizeThis();

	float GetAngle() const;
	Vec3f GetAxis() const;

	Vec4f GetVec4f() const;

	float Dot(const Quaternion &other) const;

	Quaternion operator+(const Quaternion &other) const;
	Quaternion operator-(const Quaternion &other) const;
	Quaternion operator-() const;
	
	Quaternion operator*(float scalar) const;
	Quaternion operator/(float scalar) const;

	Quaternion operator*(const Quaternion &other) const;

	const Quaternion &operator+=(const Quaternion &other);
	const Quaternion &operator-=(const Quaternion &other);

	const Quaternion &operator*=(const Quaternion &other);

	const Quaternion &operator*=(float scalar);
	const Quaternion &operator/=(float scalar);

	bool operator==(const Quaternion &other) const;
	bool operator!=(const Quaternion &other) const;

	void Rotate(float angle, const Vec3f &axis);
	void SetFromRotateDifference(const Vec3f &v1, const Vec3f &v2); 
	void SetFromMatrix(const Matrix4x4f &mat);

	static Quaternion GetRotated(float angle, const Vec3f &axis);
	static Quaternion GetRotateDifference(const Vec3f &v1, const Vec3f &v2);
	static Quaternion GetFromMatrix(const Matrix4x4f &mat);

	Quaternion Conjugate() const;
	Quaternion Inverse() const;

	void Set_Identity_Mult();
	void Set_Identity_Add();

	static Quaternion Get_Identity_Mult();
	static Quaternion Get_Identity_Add();

	Matrix4x4f GetMatrix() const;

	void SetFromEulerAngles(const Vec3f &eulerAngles);
	void SetFromRotationMatrix3x3f(const Matrix3x3f &mat);
	Vec3f GetEulerAngles() const;

	void CalculateWFromXYZ();

	void MulMatrix();

	static Quaternion Lerp(const Quaternion &first, const Quaternion &second, float interpolationCoefficient);
	static Quaternion Slerp(const Quaternion &first, const Quaternion &second, float interpolationCoefficient);
};

Vec3f operator*(const Quaternion &quat, const Vec3f &vec);
Vec3f operator*(const Vec3f &vec, const Quaternion &quat);

std::ostream &operator<<(std::ostream &output, const Quaternion &quat);