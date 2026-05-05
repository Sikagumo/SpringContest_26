#pragma once
#include <string>
#include <vector>
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Common/Quaternion.h"

constexpr float DRAW_LINE_LENGTH = 50.0f;
constexpr float DRAW_SPHERE_SIZE = 10.0f;


class UtilityMath
{

public:

	// ラジアン(rad) → 度(deg)
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);

	// 度(deg) → ラジアン(rad)
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	/* ベクトル初期値 */
	static constexpr Vector2 VECTOR2_ZERO = { 0, 0 };
	static constexpr Vector2 VECTOR2_ONE  = { 1, 1 };
	static constexpr Vector2F VECTOR2F_ZERO = { 0.0f, 0.0f };
	static constexpr Vector2F VECTOR2F_ONE  = { 1.0f, 1.0f };
	static constexpr VECTOR VECTOR_ZERO = { 0.0f, 0.0f, 0.0f };
	static constexpr VECTOR VECTOR_ONE  = { 1.0f, 1.0f, 1.0f };

	// 回転軸
	static constexpr VECTOR AXIS_X = { 1.0f, 0.0f, 0.0f };
	static constexpr VECTOR AXIS_Y = { 0.0f, 1.0f, 0.0f };
	static constexpr VECTOR AXIS_Z = { 0.0f, 0.0f, 1.0f };

	// 方向
	static constexpr VECTOR DIR_FORWARD = { 0.0f, 0.0f,  1.0f };
	static constexpr VECTOR DIR_BACK  = {  0.0f,  0.0f, -1.0f };
	static constexpr VECTOR DIR_RIGHT = {  1.0f,  0.0f,  0.0f };
	static constexpr VECTOR DIR_LEFT  = { -1.0f,  0.0f,  0.0f };
	static constexpr VECTOR DIR_UP    = {  0.0f,  1.0f,  0.0f };
	static constexpr VECTOR DIR_DOWN  = {  0.0f, -1.0f,  0.0f };

	static constexpr float kEpsilonNormalSqrt = 1e-15F;


	/// @brief 四捨五入(float→int)
	static int Round(float _value);

	/// @brief 文字列の分割
	/// @param line 
	/// @param delimiter 
	/// @return 
	static std::vector <std::string> Split(std::string& _line, char _delimiter);

	/// @brief ラジアン(rad)→度(deg)[double]
	static double Rad2Deg(double _rad) { return (_rad * RAD2DEG); };

	/// @brief ラジアン(rad)→度(deg)[float]
	static float Rad2Deg(float _rad) { return (_rad * RAD2DEG); };

	/// @brief ラジアン(rad)→度(deg)[int]
	static int Rad2Deg(int _rad) { return (_rad * Round(RAD2DEG)); };

	/// @brief ラジアン(rad)→度(deg)[vector]
	static const VECTOR& Rad2Deg(const VECTOR& _vec) { return { (_vec.x * RAD2DEG),(_vec.y * RAD2DEG),(_vec.z * RAD2DEG) }; };


	/// @brief 度(deg)→ラジアン(rad)[double]
	static double Deg2Rad(double _deg) { return (_deg * DEG2RAD); };

	/// @brief 度(deg)→ラジアン(rad)[float]
	static float Deg2Rad(float _deg) { return (_deg * DEG2RAD); };

	/// @brief 度(deg)→ラジアン(rad)[int]
	static int Deg2Rad(int _deg) { return (_deg * Round(DEG2RAD)); };

	/// @brief 度(deg)→ラジアン(rad)[vector]
	static const VECTOR& Deg2Rad(const VECTOR& _vec) { return { (_vec.x * DEG2RAD),(_vec.y * DEG2RAD),(_vec.z * DEG2RAD) }; };

	// 0～360度の範囲に収める
	static double DegIn360(double _deg);

	// 0(0)～2π(360度)の範囲に収める
	static double RadIn2PI(double rad);

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundRad(float _from, float _to);
	
	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundDeg(float _from, float _to);
	
	/* 値を範囲に収める */
	static float Clamp(float _value, float _min, float _max);

	/* ベクトルの数を範囲に収める */
	static VECTOR Clamp(const VECTOR& _target, const VECTOR& _min, const VECTOR& _max);
	static Vector2 Clamp(const Vector2& _target, const Vector2& _min, const Vector2& _max);


	// 線形補間
	static int Lerp(int _start, int _end, float _term);
	static float Lerp(float _start, float _end, float _term);
	static double Lerp(double _start, double _end, double _term);
	static Vector2 Lerp(const Vector2& _start, const Vector2& _end, float _term);
	static VECTOR Lerp(const VECTOR& _start, const VECTOR& _end, float _term);

	// 角度の線形補間
	static double LerpDeg(double _start, double _end, double _term);

	// 色の線形補間
	static COLOR_F Lerp(const COLOR_F& _start, const COLOR_F& _end, float _term);

	/* ベジェ曲線 */
	static Vector2 Bezier(const Vector2& _pos1, const Vector2& _pos2, const Vector2& _pos3, float _term);
	static VECTOR Bezier(const VECTOR& _pos1, const VECTOR& _pos2, const VECTOR& _pos3, float _term);
	
	// Y軸回転
	static VECTOR RotXZPos(const VECTOR& _centerPos, const VECTOR& _radiusPos, float _rad);

	/* ベクトルの長さ */
	static double Magnitude(const Vector2& _vec);
	static double Magnitude(const VECTOR& _vec);
	static float MagnitudeF(const VECTOR& _vec);
	static int SqrMagnitude(const Vector2& _vec);
	static float SqrMagnitudeF(const VECTOR& _vec);
	static double SqrMagnitude(const VECTOR& _vec);
	static double SqrMagnitude(const VECTOR& _vec1, const VECTOR& _vec2);
	static double Distance(const Vector2& _vec1, const Vector2& _vec2);
	static double Distance(const VECTOR& _vec1, const VECTOR& _vec2);

	// 点と球体の衝突判定
	static bool IsHitPointSphere(const VECTOR& _pos1, const VECTOR& _pos2, float _radius2);

	/// @brief 球体同士の衝突判定
	/// @param _pos1 対象１の座標
	/// @param _radius1 対象１の半径
	/// @param _pos2 対象２の座標
	/// @param _radius2 対象２の半径
	static bool IsHitSpheres(const VECTOR& _pos1, float _radius1, const VECTOR& _pos2, float _radius2);

	/// @brief 球体とカプセルの衝突判定
	/// @param _spherePos 球体の位置
	/// @param _sphereRadius 球体の半径
	/// @param _capPosTop カプセル始点の位置
	/// @param _capPosBottom カプセル終点の位置
	/// @param _capRadius カプセルの半径
	static bool IsHitSphereCapsule(const VECTOR& _spherePos, float _sphereRadius, 
								   const VECTOR& _capPosTop, const VECTOR& _capPosBottom, float _capRadius);

	/* ベクトルの比較 */
	static bool Equals(const VECTOR& _vec1, const VECTOR& _vec2);
	static bool Equals(const Vector2& _vec1, const Vector2& _vec2);
	static bool EqualsVZero(const VECTOR& _vec1);
	static bool EqualsVZero(const Vector2& _vec1);

	/* 値の範囲 */
	static bool IsRange(int _num, int _min, int _max);
	static bool IsRange(float _num, float _min, float _max);

	// 正規化
	static VECTOR Normalize(const Vector2& _vec);

	/// @brief ゼロ除算対策済みの正規化処理 
	static VECTOR VNormalize(const VECTOR& _vec);

	// @brief 2つのベクトルの間の角度
	static double AngleDeg(const VECTOR& _from, const VECTOR& _to);


	/* 方向の描画 */
	static void DrawLineDir(const VECTOR& _pos, const VECTOR& _dir, int _color, float _len = DRAW_LINE_LENGTH, float _sphereSize = DRAW_SPHERE_SIZE);
	static void DrawLineXYZ(const VECTOR& _pos, const Quaternion& _rot, float _len = DRAW_LINE_LENGTH);
	static void DrawLineXYZ(const VECTOR& _pos, const MATRIX& _rot, float _len = DRAW_LINE_LENGTH);


	/// @brief オイラー角から回転行列XYZ順を取得
	static MATRIX GetMatrixRotateXYZ(const VECTOR& _euler);

	/// @brief 親子の回転行列を合成する
	static MATRIX Multiplication(const MATRIX& _child, const MATRIX& _parent);

	/// @brief 親子のオイラー角を合成する
	static MATRIX Multiplication(const VECTOR& _childEuler, const VECTOR& _parentEuler);

	/// @brief 2D円同士の衝突判定（XY平面）
	static bool IsHitCircleXY(const VECTOR& _pos1, float _radius1,
							  const VECTOR& _pos2, float _radius2);
};

