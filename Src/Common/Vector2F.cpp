#include "Vector2F.h"


Vector2F Vector2F::operator=(const Vector2F& vec)
{
	x = vec.x;
	y = vec.y;
	return Vector2F{ vec.x, vec.y };
}


Vector2F Vector2F::operator+(const Vector2F& vec)const
{
	return Vector2F{ (x + vec.x), (y + vec.y) };
}

void Vector2F::operator+=(const Vector2F& vec)
{
	x += vec.x;
	y += vec.y;
}

Vector2F Vector2F::operator-(const Vector2F& vec)const
{
	return Vector2F{ (x - vec.x), (y - vec.y) };
}

void Vector2F::operator-=(const Vector2F& vec)
{
	x -= vec.x;
	y -= vec.y;
}

Vector2F Vector2F::operator*(const Vector2F& vec)const
{
	return Vector2F{ (x * vec.x), (y * vec.y) };
}

void Vector2F::operator*=(const Vector2F& vec)
{
	x *= vec.x;
	y *= vec.y;
}

Vector2F Vector2F::operator/(const Vector2F& vec)const
{
	return Vector2F{ (x / vec.x), (y / vec.y) };
}

void Vector2F::operator/=(const Vector2F& vec)
{
	x /= vec.x;
	y /= vec.y;
}