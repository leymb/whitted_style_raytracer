#pragma once
#include <cmath> //only used for sqrt
class vec3
{
public:

	constexpr vec3(float aX = 0.0f, float aY = 0.0f, float aZ = 0.0f);

	float x;
	float y;
	float z;

	inline vec3& Normalize();

	inline float GetMagnitude() const;
	constexpr float GetSquaredMagnitude() const;

	constexpr float DotWith(vec3& vector) const;

	constexpr vec3 CrossWith(vec3& vector) const;

	constexpr vec3 operator+(const vec3& v3) const;
	constexpr vec3 operator-(const vec3& v3) const;
	constexpr vec3 operator*(const vec3& v3) const;
	constexpr vec3 operator*(const float& other) const;
	constexpr void operator+=(const vec3& other);
	constexpr void operator-=(const vec3& other);
	constexpr void operator*=(const vec3& other);
	constexpr bool operator==(const vec3& other) const;
	constexpr bool operator!=(const vec3& other) const;
};

constexpr vec3::vec3(const float aX, const float aY, const float aZ) : x(aX), y(aY), z(aZ)
{
}

inline vec3& vec3::Normalize()
{
	const float fSquaredMagnitude = GetSquaredMagnitude();

	if (fSquaredMagnitude != 0.0f)
	{
		const auto m = 1.f / sqrtf(fSquaredMagnitude);

		*this = *this * m;

	}
	return *this;
}

inline float vec3::GetMagnitude() const
{
	return sqrtf((x * x) + (y * y) + (z * z));
}

// redundant, dot the vector with itself to get same result
constexpr float vec3::GetSquaredMagnitude() const
{
	return ((x * x) + (y * y) + (z * z));
}

constexpr float vec3::DotWith(vec3& vector) const
{
	const auto fScalar = (x * vector.x) + (y * vector.y) + (z * vector.z);
	return fScalar;
}

constexpr vec3 vec3::CrossWith(vec3& vector) const
{
	vec3 v3CrossedVector = { 0,0,0 };

	v3CrossedVector.x = (y * vector.z) - (z * vector.y);
	v3CrossedVector.y = (z * vector.x) - (x * vector.z);
	v3CrossedVector.z = (x * vector.y) - (y * vector.x);

	return v3CrossedVector;
}

constexpr vec3 vec3::operator+(const vec3& v3) const
{
	return { x + v3.x, y + v3.y, z + v3.z };
}

constexpr vec3 vec3::operator-(const vec3& v3) const
{
	return { x - v3.x, y - v3.y, z - v3.z };
}

constexpr vec3 vec3::operator*(const vec3& v3) const
{
	return { x * v3.x, y * v3.y, z * v3.z };
}

constexpr vec3 vec3::operator*(const float& f) const
{
	return { x * f, y * f, z * f };
}

constexpr void vec3::operator+=(const vec3& other)
{
	x += other.x;
	y += other.y;
	z += other.z;
}

constexpr void vec3::operator-=(const vec3& other)
{
	x -= other.x;
	y -= other.y;
	z -= other.z;
}

constexpr void vec3::operator*=(const vec3& other)
{
	x *= other.x;
	y *= other.y;
	z *= other.z;
}

constexpr bool vec3::operator==(const vec3& other) const
{
	return x == other.x && y == other.y && z == other.z;
}

constexpr bool vec3::operator!=(const vec3& other) const
{
	return !(*this == other);
}
