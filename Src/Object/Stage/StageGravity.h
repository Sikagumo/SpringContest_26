#pragma once
#include "./StageBase.h"
#include <array>
#include <map>
#include "../../CSV/CsvManager.h"
#include "../../Common/Vector2.h"

class Transform;
class StageObjBase;

class StageGravity : public StageBase
{
public:

    /// @brief コンストラクタ
	/// @param _isBack 背景ステージか否か
    StageGravity(bool _isBack);

    /// @brief デフォルトデストラクタ
    ~StageGravity(void) override = default;

	/// @brief 更新処理
    void Update(void) override; 


protected:


	/// @brief ブロック状態割り当て
	/// @param _blockType CSVのステージ配置の値
	/// @param _x 現在列数
	/// @param _y 現在行数
    StageObjBase* SetParam(int _blockType, int _x, int _y) override;

    /// @brief ブロック状態割り当て
    /// @param _blockType CSVのステージ配置の値
    /// @param _x 現在列数
    /// @param _y 現在行数
	/// @param _alpha ブロックの透明度(0.0～1.0)
	/// @param _isCollision ブロックの当たり判定があるか否か
    StageObjBase* SetParamBack(int _blockType, int _x, int _y, float _alpha = 1.0f, bool _isCollision = true)override;
};