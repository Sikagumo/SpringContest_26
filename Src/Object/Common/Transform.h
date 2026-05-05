#pragma once
#include <DxLib.h>
#include "../../Common/Quaternion.h"

/// @brief モデル制御の基本情報
/// @param 大きさ：VECTOR基準
/// @param 回転　：Quaternion基準
/// @param 位置　：VECTOR基準
class Transform
{

public:

	// モデルのハンドルID
	int modelId;

	// モデル透過度
	float alpha;

	// 大きさ
	VECTOR scl;

	// 回転
	VECTOR rot;

	// 位置
	VECTOR pos;
	VECTOR localPos;
	VECTOR prePos;

	// 行列
	MATRIX matScl;
	MATRIX matRot;
	MATRIX matPos;

	// 回転
	Quaternion quaRot;

	// ローカル回転
	Quaternion quaRotLocal;


	/// @brief デフォルトコンストラクタ
	Transform(void);

	/// @brief デフォルトデストラクタ
	~Transform(void) = default;

	/// @brief モデル制御の基本情報更新
	void Update(void);
	
	/// @brief モデルの方向を描画(デバッグ時のみ)
	void DrawDebug_ModelDir(void);

	/// @brief メモリ解放処理
	void Release(void) {};


	/// @brief 親オブジェクト割り当て
	void SetParent(Transform& _parent) { parent_ = &_parent; };

	/// @brief モデルのハンドルIDを設定
	/// @param _modelId 
	void SetModel(int _modelId);


	/// @brief モデルのスケール割り当て
	/// @param _scl 倍率(default:1.0)
	void SetScale(float _scl = 1.0f);

	/// @brief 初期化
	/// @param _scl モデルのスケール(ベクトル)
	/// @param _rot グローバル回転(クォータニオン)
	/// @param _rotLocal ローカル回転(クォータニオン)
	/// @param _pos 座標
	void InitTransform(const VECTOR& _scl,const Quaternion& _rot, const Quaternion& _rotLocal, const VECTOR& _pos);

	/// @brief 初期化処理
	/// @param _scl モデルのスケール(float)
	/// @param _rot グローバル回転(クォータニオン)
	/// @param _rotLocal ローカル回転(クォータニオン)
	/// @param _pos 座標
	void InitTransform(float _scl,const Quaternion& _rot, const Quaternion& _rotLocal, const VECTOR& _pos);

	/// @brief 初期化処理
	/// @param _scl モデルのスケール
	/// @param _rot グローバル回転(クォータニオン)
	/// @param _rotLocal ローカル回転(クォータニオン)
	void InitTransform(float _scl,const Quaternion& _rot, const Quaternion& _rotLocal);

	/// @brief 初期化処理
	void InitTransform(void);


	/// @brief 移動処理
	/// @param _movePow 移動量
	void Translate(const VECTOR& _movePow);

	/// @brief 移動処理
	/// @param _dir 移動方向
	/// @param _movePow 移動量
	void Translate(const VECTOR& _dir, float _movePow);

	/// @brief 回転処理
	/// @param _axis 回転方向
	/// @param _degPow 指定する度数分の回転量
	void Rotate(const VECTOR& _axis, float _degPow);

	/// @brief 回転処理
	/// @param _rot 回転量
	void Rotate(const Quaternion& _rot);


	/// @brief 前方方向を取得
	VECTOR GetForward(void) const;

	/// @brief 後方方向を取得
	VECTOR GetBack(void) const;

	/// @brief 右方向を取得
	VECTOR GetRight(void) const;

	/// @brief 左方向を取得
	VECTOR GetLeft(void) const;

	/// @brief 上方向を取得
	VECTOR GetUp(void) const;

	/// @brief 下方向を取得
	VECTOR GetDown(void) const;

	/// @brief 対象方向を取得
	VECTOR GetDir(const VECTOR& dir) const;

	void GetScale(float _scale) { scl = VGet(_scale, _scale, _scale); };

	/// @brief モデルの透過度割り当て
	/// @param _alpha 指定透過率(0.0～1.0)
	void SetAlpha(float _alpha);


private:

	// 親オブジェクト
	Transform* parent_;
};
