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

	enum class BLOCK_TYPE
	{
		BLANK = -1,    // 空白or未割当
		WALL,		   // 壁
		PLATER_WIDTH,  // 横移動プレイヤー
		PLATER_HEIGHT, // 縦移動プレイヤー

		GOAL, // ゴール
		TRAP, // 罠

		MAX,
	};


	StageMove(bool _isBack);

	~StageMove(void)override = default;

	void DrawDebug(void)override;


protected:

	/// @brief ブロック状態割り当て
	/// @param _blockType CSVのステージ配置の値
	/// @param _posX 現在列数
	/// @param _posY 現在行数
	StageObjBase* SetParam(int _blockType, int _x, int _y)override;
	StageObjBase* SetParamBack(int _blockType, int _x, int _y)override;
};