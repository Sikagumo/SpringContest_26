#include <string>
#include <memory>
#include <array>
#include <map>

class StatusPlayer;
class StatusEnemy;


class CsvManager
{
public:

	// 敵パラメータマップ
	using EnemyMap = std::map<int, std::unique_ptr<StatusEnemy>>;

	// ステータスの種類
	enum class STATUS_TYPE
	{
		PLAYER,
		ENEMY,

		MAX,
	};

	// セーブデータのハンドル
	const char* PATH_PLAYER = "PlayerData.csv";
	const char* PATH_ENEMY = "EnemyData.csv";


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
	void SaveCSV(void);

	
	/// @brief プレイヤーステータス参照
	StatusPlayer& GetPlayerStatus(void)const { return *player_; };

	/// @brief 敵ステータス参照
	StatusEnemy& GetEnemyStatus(int target)const { return *enemy_.at(target - 1).get(); };

	/// @brief 敵ステータスマップ参照
	const EnemyMap& GetEnemyStatusMap(void)const { return enemy_; };

	
	std::string& GetHandlePathPlayer(void);
	std::string& GetHandlePathEnemy(int type);

private:

	// インスタンス
	static CsvManager* instance_;

	// プレイヤーパラメータ
	std::unique_ptr<StatusPlayer> player_;
	
	// 敵パラメータ
	EnemyMap enemy_;

	
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
	void LoadEnemy(void);
};