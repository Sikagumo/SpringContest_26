#pragma once
#include "./ColliderBase.h"
#include <vector>
#include <string>

class ColliderModel : public ColliderBase
{
public:

	/// @brief コンストラクタ
	/// @param _tag タグ対象
	/// @param _follow 追従対象
	ColliderModel(TAG _tag, const Transform * _follow);

	~ColliderModel(void) override = default;


	/// @brief 指定された文字を含むフレームを衝突判定から除外
	void AddExcludeFrameIds(const std::string& _name);

	/// @brief 衝突判定から除外するフレームをクリア
	void ClearExcludeFrame(void);

	/// @brief 指定した除外フレームがあるか否か
	bool IsExcludeFrame(int _frameIdx) const;

	/// @brief 指定された文字を含むフレームを衝突判定対象とする
	/// @param _name 
	void AddTargetFrameIds(const std::string& _name);

	/// @brief 衝突判定の対象するフレームをクリア
	void ClearTargetFrame(void);

	/// @brief 対象フレームがあるか否か
	bool IsTargetFrame(int _frameIdx) const;

	/// @brief 線分とモデルの最近接(startに近い)衝突ポリゴンを取得
	/// @param _start 始点
	/// @param _end 終点
	/// @param _isExclude 除外するか否か
	/// @param _isTarget 
	MV1_COLL_RESULT_POLY GetNearestHitPolyLine(const VECTOR& _start, const VECTOR& _end,
											   bool _isExclude = false, bool _isTarget = false) const;


protected:

	// 衝突判定から除外するフレーム番号
	std::vector<int> excludeFrameIds_;

	// 衝突判定の対象とするフレーム番号
	std::vector<int> targetFrameIds_;


	/// @brief デバッグ描画
	/// @param _color 
	void DrawDebug(unsigned int _color) override {};


};