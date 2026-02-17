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
		BLANK = -1,
		WALL,
		PLATER_WIDTH,
		PLATER_HEIGHT,
		GOAL,

		MAX,
	};


	StageMove(void);

	~StageMove(void)override = default;


	void Init(void)override;

	void Update(void)override;

	void Draw(void)override;
	void DrawDebug(void)override;

	void Release(void)override;


private:


	static constexpr VECTOR STAGE_POS = { -1000.0f, -1000.0f, 850.0f };
	static constexpr float BLOCK_SCALE = 1.0f;

	static constexpr float BLOCK_OFFSET_X = 200.0f;
	static constexpr float BLOCK_OFFSET_Y = 200.0f;

	struct BlockParam
	{
		BlockParam(void) :
			type(BLOCK_TYPE::BLANK),
			viewParam(nullptr),
			collisionPos(0, 0),
			collisionSize(0, 0) {
		};

		BLOCK_TYPE type;
		Transform* viewParam;
		Vector2 collisionPos;
		Vector2 collisionSize;
	};

	// 配置リスト
	std::vector<std::vector<BlockParam*>> placeType_;

	std::vector<BlockParam*> SetBlockType(int _type, int list);
};