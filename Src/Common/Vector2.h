#pragma once

/// @brief int”ÅVector2
class Vector2
{
public:

	int x, y;

	~Vector2(void) = default;


	/* ‘ã“üˆ— */
	Vector2 operator=(const Vector2& _vec);

	/* ‰ÁZˆ— */
	Vector2 operator+(const Vector2& vec)const;
	void operator+=(const Vector2& vec);

	/* Œ¸Zˆ— */
	Vector2 operator-(const Vector2& vec)const;
	void operator-=(const Vector2& vec);

	/* æZˆ— */
	Vector2 operator*(const Vector2& vec)const;
	void operator*=(const Vector2& vec);

	/* œZˆ— */
	Vector2 operator/(const Vector2& vec)const;
	void operator/=(const Vector2& vec);
};


/// @brief float”ÅVector2
class Vector2F
{
public:

	float x, y;


	~Vector2F(void) = default;

	/* ‘ã“üˆ— */
	Vector2F operator=(const Vector2F& _vec);

	/* ‰ÁZˆ— */
	Vector2F operator+(const Vector2F& _vec)const;
	void operator+=(const Vector2F& _vec);

	/* Œ¸Zˆ— */
	Vector2F operator-(const Vector2F& _vec)const;
	void operator-=(const Vector2F& _vec);

	/* æZˆ— */
	Vector2F operator*(const Vector2F& _vec)const;
	void operator*=(const Vector2F& _vec);

	/* œZˆ— */
	Vector2F operator/(const Vector2F& _vec)const;
	void operator/=(const Vector2F& _vec);
};