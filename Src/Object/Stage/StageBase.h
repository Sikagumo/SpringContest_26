#pragma once
#include <vector>
#include "../../Common/Vector2.h"
#include "../Common/Transform.h"
#include "../Actor/ActorBase.h"

class StageObjBase;

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

	const VECTOR& GetPlayerPos(int _num) { return playersPos_[_num]; };

	const VECTOR& GetGoalPos(void) { return goalPos_; };

	/// @brief ステージの当たり判定を全登録
	/// @param _actor 割り当てる対象
	void AddStageColliders(ActorBase& _actor);


protected:

	SceneManager& sceneMng_;
	ResourceManager& resMng_;


	// 配置リスト
	std::vector<std::vector<StageObjBase*>> placeType_;

	// プレイヤー初期位置
	VECTOR playersPos_[2];
	VECTOR goalPos_;


	virtual void InitList(void) = 0;

	/// @brief ブロック配置処理
	/// @param _type マップの種類
	/// @param _xMax 列数
	/// @param _yMax 行数
	/// @return ブロックの行リスト
	void SetBlockTypeList(int _type, int _xMax, int _yMax);

	/// @brief ブロック状態割り当て
	/// @param _blockType CSVのステージ配置の値
	/// @param _posX 現在列数
	/// @param _posY 現在行数
	virtual StageObjBase* SetParam(int _blockType, float _posX, float _posY) = 0;
};