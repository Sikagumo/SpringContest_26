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

		MAX,
	};


	StageMove(void);

	~StageMove(void)override = default;

	void DrawDebug(void)override;

protected:


	static constexpr VECTOR STAGE_POS = { -1000.0f, -1000.0f, 850.0f };
	static constexpr float BLOCK_SCALE = 1.0f;

	static constexpr VECTOR BLOCK_OFFSET = { 200.0f, 200.0f, 100.0f };


	/// @brief ブロック状態割り当て
	/// @param _blockType CSVのステージ配置の値
	/// @param _posX 現在列数
	/// @param _posY 現在行数
	StageObjBase* SetParam(int _blockType, float _posX, float _posY)override;
	StageObjBase* SetParamBack(int _blockType, float _posX, float _posY)override;
};