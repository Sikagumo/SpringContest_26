#pragma once
#include "./StageBase.h"
#include <array>
#include <map>
#include "../../CSV/CsvManager.h"
#include "../../Common/Vector2.h"
class Transform;

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

	static constexpr float BLOCK_OFFSET_X = 200.0f;
	static constexpr float BLOCK_OFFSET_Y = 200.0f;


	void InitList(void)override;

	/// @brief ブロック状態割り当て
	/// @param _param ブロックパラメータ
	/// @param _blockType CSVのステージ配置の値
	/// @param _posX 現在列数
	/// @param _posY 現在行数
	void SetParam(StageBase::BlockParam& _param, int _blockType, float _posX, float _posY)override;
};