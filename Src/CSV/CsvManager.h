#pragma once
#include <string>
#include <memory>
#include <array>
#include <vector>

class CsvManager
{
public:

	// セーブデータのハンドル
	const char* PATH_STAGE_MOVE	   = "StageMove.csv";
	const char* PATH_STAGE_GRAVITY = "StageGravity.csv";

	// ステージサイズ(ラベルは含めない)
	static constexpr int STAGE_X = 11;
	static constexpr int STAGE_Y = 11;


	static void CreateInstance(void);
	static CsvManager& GetInstance(void) { return *instance_; };
	static void DestroyInstance(void);

	void Load(void);


	/// @brief 移動ステージのマップ情報取得
	/// @param _type ステージの種類 
	/// @param x ステージの取得する横位置
	/// @param y ステージの取得する縦位置
	int GetStageMoveNum(int _type, int x, int y);

	/// @brief 移動ステージの3Dの後ろマップ情報取得
	/// @param _type ステージの種類
	/// @param x ステージの取得する横位置
	/// @param y ステージの取得する縦位置
	int GetStageMoveBackNum(int _type, int x, int y);

	
	/// @brief 重力ステージのマップ情報取得
	/// @param _type ステージの種類
	/// @param x ステージの取得する横位置
	/// @param y ステージの取得する縦位置
	int GetStageGravityNum(int _type, int x, int y);

	/// @brief 重力ステージの3Dの後ろマップ情報取得
	/// @param _type ステージの種類
	/// @param x ステージの取得する横位置
	/// @param y ステージの取得する縦位置
	int GetStageGravityBackNum(int _type, int _x, int _y);


	/* ステージ数取得 */
	int GetStageMoveMapNum(void) { return static_cast<int>(stage_.move.size()); };
	int GetStageGravityMapNum(void) { return static_cast<int>(stage_.gravity.size()); };


private:

	static CsvManager* instance_;

	struct StageMap
	{
		StageMap(void)
			: move{}, moveBack{}
			, gravity{}, gravityBack{}
		{}

		/* 移動ステージ配置リスト */
		using MoveStagePlace = std::array<std::array<int, STAGE_X>, STAGE_Y>;
		std::vector<MoveStagePlace> move;
		std::vector<MoveStagePlace> moveBack;

		/* 重力ステージ配置リスト */
		using GravityStagePlace = std::array<std::array<int, STAGE_X>, STAGE_Y>;
		std::vector<GravityStagePlace> gravity;
		std::vector<GravityStagePlace> gravityBack;

	};

	StageMap stage_;

	
	CsvManager(void);
	~CsvManager(void) = default;

	/* コピーコンストラクタ対策 */
	CsvManager(const CsvManager&)			 = delete;
	CsvManager& operator=(const CsvManager&) = delete;
	CsvManager(CsvManager&&)			= delete;
	CsvManager& operator=(CsvManager&&) = delete;

	/// @brief ビルド別ファイル読み込み処理
	/// @param _path 指定CSVファイルパス
	std::string ReadCsvFile(const std::string& _path);

	/// @brief 移動ステージ読み込み処理
	/// @param _path 読み込むCSVファイルのパス
	/// @param _isLabelSkip ファイル内のステージ別のラベルを読み込まないか否か
	void LoadStageMoveCsv(const std::string& _path, bool _isLabelSkip);

	/// @brief 重力ステージ読み込み処理
	/// @param _path 読み込むCSVファイルのパス
	/// @param _isLabelSkip ファイル内のステージ別のラベルを読み込まないか否か
	void LoadStageGravityCsv(const std::string& _path, bool _isLabelSkip);
};