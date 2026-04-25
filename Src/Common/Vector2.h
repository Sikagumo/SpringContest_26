#pragma once

/// @brief int版Vector2
class Vector2
{
public:

	int x; // X座標
	int y; // Y座標

	/// @brief デフォルトデストラクタ
	~Vector2(void) = default;


	/* 代入処理 */
	Vector2 operator=(const Vector2& vec);

	/* 加算処理 */
	Vector2 operator+(const Vector2& vec)const;
	void operator+=(const Vector2& vec);

	/* 減算処理 */
	Vector2 operator-(const Vector2& vec)const;
	void operator-=(const Vector2& vec);

	/* 乗算処理 */
	Vector2 operator*(const Vector2& vec)const;
	void operator*=(const Vector2& vec);

	/* 除算処理 */
	Vector2 operator/(const Vector2& vec)const;
	void operator/=(const Vector2& vec);
};


/// @brief float版Vector2
class Vector2F
{
public:

	float x; // X座標
	float y; // Y座標

	/// @brief デフォルトデストラクタ
	~Vector2F(void) = default;

	/* 代入処理 */
	Vector2F operator=(const Vector2F& vec);

	/* 加算処理 */
	Vector2F operator+(const Vector2F& vec)const;
	void operator+=(const Vector2F& vec);

	/* 減算処理 */
	Vector2F operator-(const Vector2F& vec)const;
	void operator-=(const Vector2F& vec);

	/* 乗算処理 */
	Vector2F operator*(const Vector2F& vec)const;
	void operator*=(const Vector2F& vec);

	/* 除算処理 */
	Vector2F operator/(const Vector2F& vec)const;
	void operator/=(const Vector2F& vec);
};