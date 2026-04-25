#pragma once
#include <vector>
#include <array>
#include <DxLib.h>
#include "../../Utility/UtilityCommon.h"
#include "../../Common/Vector2.h"
#include "../Common/Transform.h"
#include "../Actor/ActorBase.h"

class StageObjBase;
class CsvManager;

class StageBase
{
public:

	enum class BLOCK_TYPE
	{
		BLANK = -1,    // 空白or未割当
		WALL,		   // 壁
		PLAYER_WIDTH,  // 横プレイヤー
		PLAYER_HEIGHT, // 縦プレイヤー

		GOAL, // ゴール
		TRAP, // 罠
		TRAP_VEERTICAL_MOVE, //縦移動罠
		TRAP_HORIZONTAL_MOVE, //横移動罠

		MAX,
	};

	enum class TYPE
	{
		MOVE,
		MOVE3D,
		GRAVITY,
		GRAVITY3D,
	};

	StageBase(TYPE stageType, int mapNum, int mapBackNum = 0);

	virtual ~StageBase(void) = default;


	void Init(int _stageNum);

	virtual void Update(void) = 0;

	virtual void Draw(void);

	void DrawPre(void);

	virtual void Release(void);


	const VECTOR& GetPlayerPos(int _num) { return initialPlayersPos_[_num]; };

	const VECTOR& GetGoalPos(void) { return goalPos_; };
	const VECTOR& GetGoalBackPos(void) { return goalPosBack_; };

	//外部空罠のリストを取得する関数
	const std::vector<VECTOR>& GetTrapPos(void) const { return trapPositions_; }


	const std::vector<std::vector<StageObjBase*>>& GetPlaceType(void) const { return placeFrontList_; }
	const std::vector<std::vector<StageObjBase*>>& GetPlaceBackType(void) const { return placeBackList_; }

	/// @brief ステージの当たり判定を全登録
	/// @param _actor 割り当てる対象1
	void AddStageColliders(ActorBase& _actor);

	int GetStageNum(void) { return curStageNum_; };


protected:

	static constexpr UtilityCommon::Color WALL_COLOR_FLONT = UtilityCommon::Color(255, 255, 0, 255);
	static constexpr UtilityCommon::Color WALL_COLOR_BACK = UtilityCommon::Color(0, 255, 0, 255);
	static constexpr UtilityCommon::Color WALL_COLOR_BACKGROUND = UtilityCommon::Color(255, 200, 255, 255);

	static constexpr VECTOR STAGE_POS = { -1000.0f, -1000.0f, 850.0f };
	static constexpr VECTOR BLOCK_OFFSET = { 199.9f, 199.9f, 200.0f };
	static constexpr float BLOCK_SCALE = 1.0f;
	static constexpr float PLAYER_OFFSET_Z = -10.0f;

	static constexpr float FRONT_ALPHA = 0.75f;
	static constexpr float BACK_ALPHA = 0.45f;

	SceneManager& sceneMng_;
	ResourceManager& resMng_;
	CsvManager& csvMng_;
	
	int curStageNum_;

	TYPE curStageType_;

	// マップ最大数
	int mapNumMax_;
	int mapBackNumMax_;

	// 前配置リスト
	std::vector<std::vector<StageObjBase*>> placeFrontList_;

	// 後ろ配置リスト
	std::vector<std::vector<StageObjBase*>> placeBackList_;

	// 後ろ配置の空白リスト
	std::vector<StageObjBase*> placeBackBlankList_;

	// 背景配置リスト
	std::vector<StageObjBase*> backGroundList_;

	// プレイヤー初期位置
	std::array<VECTOR, 2> initialPlayersPos_;

	// ゴールの前配置
	VECTOR goalPos_;

	// ゴールの後ろ配置
	VECTOR goalPosBack_;

	
	// 罠の座標を格納するリストを追加
	std::vector<VECTOR>trapPositions_;
	

	/// @brief ステージ指定処理
	/// @param _stageNum 指定番号
	void StageChoice(int _stageNum);

	/// @brief ブロック配置処理
	/// @param _mapType マップの種類
	/// @param _xMax 列最大数
	/// @param _yMax 行最大数
	void SetBlockTypeList(int _mapType, int _xMax, int _yMax);

	/// @brief ブロック配置処理
	/// @param _mapType マップの種類
	/// @param _xMax 列最大数
	/// @param _yMax 行最大数
	void SetBlockBackTypeList(int _mapType, int _xMax, int _yMax);

	/// @brief ブロック背景配置処理
	/// @param _xMax 列最大数
	/// @param _yMax 行最大数
	void SetBackGroundList(int _xMax, int _yMax);


	/// @brief ブロック状態割り当て
	/// @param _blockType CSVのステージ配置の値
	/// @param _x 現在列数
	/// @param _y 現在行数
	virtual StageObjBase* SetParam(int _blockType, int _x, int _y) = 0;

	/// @brief ブロック状態割り当て
	/// @param _blockType CSVのステージ配置の値
	/// @param _x 現在列数
	/// @param _y 現在行数
	/// @param _alpha ブロックの透明度(0.0～1.0)
	/// @param _isCollision ブロックの当たり判定があるか否か
	virtual StageObjBase* SetParamBack(int _blockType, int _x, int _y, float _alpha = 1.0f, bool _isCollision = true) = 0;

	/// @brief 前が壁かつ後ろが壁以外の場合、前の壁を透過させる
	void ChangeFrontObjects(void);

	/// @brief  後ろステージに同座標の壁が存在するか否か
	/// @param _pos 判定する位置
	bool _IsHasBackWallAt(const Vector2& _pos)const;
};