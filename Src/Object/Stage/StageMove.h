#pragma once
#include "./StageBase.h"
#include <array>
#include <map>
#include "../../CSV/CsvManager.h"
#include "../../Common/Vector2.h"

class Transform;
class StageObjBase;

class StageMove : public StageBase
{
public:


	StageMove(bool _isBack);

	~StageMove(void)override = default;

	void Update(void) override;

protected:

	/// @brief ブロック状態割り当て
	/// @param _blockType CSVのステージ配置の値
	/// @param _posX 現在列数
	/// @param _posY 現在行数
	StageObjBase* SetParam(int _blockType, int _x, int _y)override;
	StageObjBase* SetParamBack(int _blockType, int _x, int _y, float _alpha = 1.0f, bool _isCollision = true)override;
};