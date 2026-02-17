#pragma once
#include <string>
#include <memory>
#include <array>
#include <vector>

class StatusPlayer;

class CsvManager
{
public:

	// セーブデータのハンドル
	const char* PATH_PLAYER = "PlayerData.csv";
	const char* PATH_STAGE_MOVE = "StageMove.csv";
	const char* PATH_STAGE_GRAVITY = "StageGravity.csv";

	// 移動ステージサイズ(ラベルは含めない)
	static constexpr int STAGE_MOVE_X = 11;
	static constexpr int STAGE_MOVE_Y = 11;

	// 重力ステージサイズ
	static constexpr int STAGE_GRAVITY_X = 10;
	static constexpr int STAGE_GRAVITY_Y = 10;


	/// @brief インスタンス生成処理
	static void CreateInstance(void);

	/// @brief インスタンス取得処理
	static CsvManager& GetInstance(void);

	/// @brief インスタンス削除処理
	static void Destroy(void);

	/// @brief 初回読み込み処理
	void Load(void);

	/// @brief .csvファイル書き出し処理
	/// @returns 正常動作できたか否か
	//void SaveCSV(void);

	/// @brief プレイヤーステータス参照
	StatusPlayer& GetPlayerStatus(void)const { return *player_; };
	
	std::string& GetHandlePathPlayer(void);


	int GetStageMoveNum(int _type, int x, int y);

	int GetStageGravityNum(int _type, int x, int y);

private:

	// インスタンス
	static CsvManager* instance_;

	// プレイヤーパラメータ
	std::unique_ptr<StatusPlayer> player_;

	struct StageMap
	{
		// 移動ステージ配置リスト
		using MoveStagePlace = std::array<std::array<int, STAGE_MOVE_X>, STAGE_MOVE_Y>;
		std::vector<MoveStagePlace> move;

		// 重力ステージ配置リスト
		using GravityStagePlace = std::array<std::array<int, STAGE_GRAVITY_X>, STAGE_GRAVITY_Y>;
		std::vector<GravityStagePlace> gravity;
	};

	StageMap stage_;

	
	/// @brief デフォルトコンストラクタ
	CsvManager(void);

	/// @brief デフォルトデストラクタ
	~CsvManager(void) = default;

	// コピーコンストラクタ対策
	CsvManager(const CsvManager&) = delete;
	CsvManager& operator=(const CsvManager&) = delete;
	CsvManager(CsvManager&&) = delete;
	CsvManager& operator=(CsvManager&&) = delete;


	std::string ReadCsvFile(const std::string& _path);

	// 各csvファイル読み込み
	void LoadPlayerStatus(void);

	void LoadStages(void);

	void LoadStageMoveCsv(const std::string& _path, int _xSize,  int _ySize, bool _isLabelSkip);
	void LoadStageGravityCsv(const std::string& _path, int _xSize,  int _ySize, bool _isLabelSkip);
};