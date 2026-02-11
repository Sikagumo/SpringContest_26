#pragma once
#include <string>
#include <vector>
#include <DxLib.h>
#include "../Common/Vector2.h"
#include "../Common/Quaternion.h"
class AsoUtility
{

public:

	// ラジアン(rad) → 度(deg)
	static constexpr float RAD2DEG = (180.0f / DX_PI_F);

	// 度(deg) → ラジアン(rad)
	static constexpr float DEG2RAD = (DX_PI_F / 180.0f);

	static constexpr Vector2 VECTOR2_ZERO = { 0, 0 };
	static constexpr Vector2 VECTOR2_ONE  = { 1, 1 };

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


	// 四捨五入
	static int Round(float v);

	// 文字列の分割
	static std::vector <std::string> Split(std::string& line, char delimiter);

	/// @brief ラジアン(rad)から度(deg)[double]
	static double Rad2Deg(double rad) { return (rad * RAD2DEG); };

	/// @brief ラジアン(rad)から度(deg)[float]
	static float Rad2Deg(float rad) { return (rad * RAD2DEG); };

	/// @brief ラジアン(rad)から度(deg)[int]
	static int Rad2Deg(int rad) { return (rad * Round(RAD2DEG)); };

	/// @brief ラジアン(rad)から度(deg)[vector]
	static const VECTOR& Rad2Deg(const VECTOR& _vec) { return { (_vec.x * RAD2DEG),(_vec.y * RAD2DEG),(_vec.z * RAD2DEG) }; };


	/// @brief 度(deg)→ラジアン(rad)[double]
	static double Deg2Rad(double deg) { return (deg * DEG2RAD); };

	/// @brief 度(deg)→ラジアン(rad)[float]
	static float Deg2Rad(float deg) { return (deg * DEG2RAD); };

	/// @brief 度(deg)→ラジアン(rad)[int]
	static int Deg2Rad(int deg) { return (deg * Round(DEG2RAD)); };

	/// @brief 度(deg)→ラジアン(rad)[vector]
	static const VECTOR& Deg2Rad(const VECTOR& _vec) { return { (_vec.x * DEG2RAD),(_vec.y * DEG2RAD),(_vec.z * DEG2RAD) }; };

	// 0～360度の範囲に収める
	static double DegIn360(double deg);

	// 0(0)～2π(360度)の範囲に収める
	static double RadIn2PI(double rad);

	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundRad(float from, float to);
	
	// 回転が少ない方の回転向きを取得する(時計回り:1、反時計回り:-1)
	static int DirNearAroundDeg(float from, float to);
	
	// 値を範囲に収める
	static float Clamp(float v, float min, float max);

	// ベクトルの数を範囲に収める
	static VECTOR Clamp(const VECTOR& _target, const VECTOR& _min, const VECTOR& _max);
	static Vector2 Clamp(const Vector2& _target, const Vector2& _min, const Vector2& _max);


	// 線形補間
	static int Lerp(int start, int end, float t);
	static float Lerp(float start, float end, float t);
	static double Lerp(double start, double end, double t);
	static Vector2 Lerp(const Vector2& start, const Vector2& end, float t);
	static VECTOR Lerp(const VECTOR& start, const VECTOR& end, float t);

	// 角度の線形補間
	static double LerpDeg(double start, double end, double t);

	// 色の線形補間
	static COLOR_F Lerp(const COLOR_F& start, const COLOR_F& end, float t);

	// ベジェ曲線
	static Vector2 Bezier(const Vector2& p1, const Vector2& p2, const Vector2& p3, float t);
	static VECTOR Bezier(const VECTOR& p1, const VECTOR& p2, const VECTOR& p3, float t);
	
	// Y軸回転
	static VECTOR RotXZPos(const VECTOR& centerPos, const VECTOR& radiusPos, float rad);

	// ベクトルの長さ
	static double Magnitude(const Vector2& v);
	static double Magnitude(const VECTOR& v);
	static float MagnitudeF(const VECTOR& v);
	static int SqrMagnitude(const Vector2& v);
	static float SqrMagnitudeF(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v);
	static double SqrMagnitude(const VECTOR& v1, const VECTOR& v2);
	static double Distance(const Vector2& v1, const Vector2& v2);
	static double Distance(const VECTOR& v1, const VECTOR& v2);

	// 点と球体の衝突判定
	static bool IsHitPointSphere(
		const VECTOR& pos1, const VECTOR& pos2, float radius2);

	// 球体同士の衝突判定
	static bool IsHitSpheres(
		const VECTOR& pos1, float radius1, const VECTOR& pos2, float radius2);

	// 球体とカプセルの衝突判定
	static bool IsHitSphereCapsule(
		const VECTOR& sphPos, float sphRadius, 
		const VECTOR& capPos1, const VECTOR& capPos2, float capRadius);

	// 比較
	static bool Equals(const VECTOR& v1, const VECTOR& v2);
	static bool Equals(const Vector2& v1, const Vector2& v2);
	static bool EqualsVZero(const VECTOR& v1);
	static bool EqualsVZero(const Vector2& v1);

	// 値の範囲
	static bool IsRange(int _num, int _min, int _max);
	static bool IsRange(float _num, float _min, float _max);

	// 正規化
	static VECTOR Normalize(const Vector2& v);

	/// @brief ゼロ除算対策済みの正規化処理 
	static VECTOR VNormalize(const VECTOR& v);

	// @brief 2つのベクトルの間の角度
	static double AngleDeg(const VECTOR& from, const VECTOR& to);

	// 描画系
	static void DrawLineDir(const VECTOR& pos, const VECTOR& dir, int color, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const Quaternion& rot, float len = 50.0f);
	static void DrawLineXYZ(const VECTOR& pos, const MATRIX& rot, float len = 50.0f);


	// オイラー角から回転行列XYZ順を取得
	static MATRIX GetMatrixRotateXYZ(const VECTOR& euler);

	// 親子の回転行列を合成する
	static MATRIX Multiplication(const MATRIX& child, const MATRIX& parent);

	// 親子のオイラー角を合成する
	static MATRIX Multiplication(const VECTOR& childEuler, const VECTOR& parentEuler);

};

