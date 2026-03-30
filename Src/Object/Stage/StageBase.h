#pragma once
#include <vector>
#include "../../Common/Vector2.h"
#include "../Common/Transform.h"
#include "../Actor/ActorBase.h"

class StageObjBase;
class CsvManager;

class StageBase
{
public:

	enum class TYPE
	{
		MOVE,
		MOVE3D,
		GRAVITY,
		GRAVITY3D,
	};
	StageBase(TYPE stageType, int mapNum, int mapBackNum = 0);

	virtual ~StageBase(void) = default;


	virtual void Init(void);

	virtual void Update(void);

	virtual void Draw(void);

	void DrawPre(void);

	virtual void DrawDebug(void) {};

	virtual void Release(void);

	/// @brief ステージ再抽選処理
	/// @param _exclusionList ステージ番号の除外リスト
	void StageReChoice(std::vector<int> _exclusionList);

	const VECTOR& GetPlayerPos(int _num) { return playersPos_[_num]; };

	const VECTOR& GetGoalPos(void) { return goalPos_; };
	const VECTOR& GetGoalBackPos(void) { return goalPosBack_; };

	//外部空罠のリストを取得する関数
	const std::vector<VECTOR>& GetTrapPos(void) const { return trapPositions_; }

	/// @brief ステージの当たり判定を全登録
	/// @param _actor 割り当てる対象1
	void AddStageColliders(ActorBase& _actor);

	int GetStageNum(void) { return stageNum_; };


protected:

	static constexpr VECTOR STAGE_POS = { -1000.0f, -1000.0f, 850.0f };
	static constexpr VECTOR BLOCK_OFFSET = { 199.9f, 199.9f, 200.0f };
	static constexpr float BLOCK_SCALE = 1.0f;
	static constexpr float PLAYER_OFFSET_Z = -10.0f;

	static constexpr float FLONT_ALPHA = 0.65f;
	static constexpr float BACK_ALPHA = 0.85f;

	SceneManager& sceneMng_;
	ResourceManager& resMng_;
	CsvManager& csvMng_;
	
	int stageNum_;

	TYPE stageType_;

	// マップ最大数
	int mapNumMax_;
	int mapBackNumMax_;

	// 配置リスト
	std::vector<std::vector<StageObjBase*>> placeType_;
	std::vector<std::vector<StageObjBase*>> placeBackType_;
	std::vector<std::vector<StageObjBase*>> backGroundList_;

	// プレイヤー初期位置
	VECTOR playersPos_[2];

	//罠の配置
	VECTOR goalPos_;
	VECTOR goalPosBack_;

	
	//罠の座標を格納するリストを追加
	std::vector<VECTOR>trapPositions_;
	
	/// @brief ステージ指定処理
	/// @param _stageNum 指定番号
	void StageChoice(int _stageNum);

	/// @brief ブロック配置処理
	/// @param _type マップの種類
	/// @param _xMax 列数
	/// @param _yMax 行数
	/// @return ブロックの行リスト
	void SetBlockTypeList(int _mapType, int _xMax, int _yMax);
	void SetBlockBackTypeList(int _mapType, int _xMax, int _yMax);
	void SetBlockBackList(int _xMax, int _yMax);

	/// @brief ブロック状態割り当て
	/// @param _blockType CSVのステージ配置の値
	/// @param _posX 現在列数
	/// @param _posY 現在行数
	virtual StageObjBase* SetParam(int _mapType, int _x, int _y) = 0;
	virtual StageObjBase* SetParamBack(int _mapType, int _x, int _y) = 0;

	void ChangeFrontObjects(int _stageType);
};