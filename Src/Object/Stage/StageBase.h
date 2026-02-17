#pragma once
#include <vector>
#include "../../Common/Vector2.h"
#include "../Common/Transform.h"
class SceneManager;
class ResourceManager;

class StageBase
{
public:

	StageBase(void);

	virtual ~StageBase(void) = default;

	virtual void Init(void);
	virtual void Update(void);
	virtual void Draw(void);
	virtual void DrawDebug(void) {};
	virtual void Release(void);

protected:

	SceneManager& sceneMng_;
	ResourceManager& resMng_;

	struct BlockParam
	{
		BlockParam(void) :
			type(-1),
			viewParam(nullptr),
			collisionPosX(0.0f),
			collisionPosY(0.0f),
			collisionSize(0, 0) {
		};

		int type;
		Transform* viewParam;
		float collisionPosX;
		float collisionPosY;
		Vector2 collisionSize;
	};

	// 配置リスト
	std::vector<std::vector<BlockParam*>> placeType_;


	virtual void InitList(void) = 0;

	/// @brief ブロック配置処理
	/// @param _type マップの種類
	/// @param _xMax 列数
	/// @param _yMax 行数
	/// @return ブロックの行リスト
	void SetBlockTypeList(int _type, int _xMax, int _yMax);

	/// @brief ブロック状態割り当て
	/// @param _param ブロックパラメータ
	/// @param _blockType CSVのステージ配置の値
	/// @param _posX 現在列数
	/// @param _posY 現在行数
	virtual void SetParam(BlockParam& _param, int _blockType, float _posX, float _posY) = 0;
};