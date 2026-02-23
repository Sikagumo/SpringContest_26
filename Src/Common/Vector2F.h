#pragma once

/// @brief float版Vector2
class Vector2F
{
public:

	float x; // X座標
	float y; // Y座標

	// デストラクタ
	~Vector2F(void) = default;

	// 代入処理
	Vector2F operator=(const Vector2F& vec);

	// 加算処理
	Vector2F operator+(const Vector2F& vec)const;
	void operator+=(const Vector2F& vec);

	// 減算処理
	Vector2F operator-(const Vector2F& vec)const;
	void operator-=(const Vector2F& vec);

	// 乗算処理
	Vector2F operator*(const Vector2F& vec)const;
	void operator*=(const Vector2F& vec);

	// 除算処理
	Vector2F operator/(const Vector2F& vec)const;
	void operator/=(const Vector2F& vec);
};