#include "Vector2.h"

/* --- <<< int”ÔVector2 >>> --- */

Vector2 Vector2::operator=(const Vector2& _vec)
{
	x = _vec.x;
	y = _vec.y;
	return Vector2{ _vec.x, _vec.y };
}


Vector2 Vector2::operator+(const Vector2& _vec)const
{
	return Vector2{ (x + _vec.x), (y + _vec.y) };
}

void Vector2::operator+=(const Vector2& _vec)
{
	x += _vec.x;
	y += _vec.y;
}

Vector2 Vector2::operator-(const Vector2& _vec)const
{
	return Vector2{ (x - _vec.x), (y - _vec.y) };
}

void Vector2::operator-=(const Vector2& _vec)
{
	x -= _vec.x;
	y -= _vec.y;
}

Vector2 Vector2::operator*(const Vector2& _vec)const
{
	return Vector2{ (x * _vec.x), (y * _vec.y) };
}

void Vector2::operator*=(const Vector2& _vec)
{
	x *= _vec.x;
	y *= _vec.y;
}

Vector2 Vector2::operator/(const Vector2& _vec)const
{
	return Vector2{ (x / _vec.x), (y / _vec.y) };
}

void Vector2::operator/=(const Vector2& _vec)
{
	x /= _vec.x;
	y /= _vec.y;
}


/* --- <<< flaot”ÅVector2 >>> --- */

Vector2F Vector2F::operator=(const Vector2F& _vec)
{
	x = _vec.x;
	y = _vec.y;
	return Vector2F{ _vec.x, _vec.y };
}


Vector2F Vector2F::operator+(const Vector2F& _vec)const
{
	return Vector2F{ (x + _vec.x), (y + _vec.y) };
}

void Vector2F::operator+=(const Vector2F& _vec)
{
	x += _vec.x;
	y += _vec.y;
}

Vector2F Vector2F::operator-(const Vector2F& _vec)const
{
	return Vector2F{ (x - _vec.x), (y - _vec.y) };
}

void Vector2F::operator-=(const Vector2F& _vec)
{
	x -= _vec.x;
	y -= _vec.y;
}

Vector2F Vector2F::operator*(const Vector2F& _vec)const
{
	return Vector2F{ (x * _vec.x), (y * _vec.y) };
}

void Vector2F::operator*=(const Vector2F& _vec)
{
	x *= _vec.x;
	y *= _vec.y;
}

Vector2F Vector2F::operator/(const Vector2F& _vec)const
{
	return Vector2F{ (x / _vec.x), (y / _vec.y) };
}

void Vector2F::operator/=(const Vector2F& _vec)
{
	x /= _vec.x;
	y /= _vec.y;
}