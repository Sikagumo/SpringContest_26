#pragma once

#include <unordered_map>
#include <string>
#include "./Resource.h"

class ResourceManager
{
public:

	/// @brief リソース識別列挙
	enum class SRC
	{
		NONE = -1,
		
		// 画像
		IMG_TITLE,
		IMG_SHADOW,
		IMG_PUSH_SPACE,
		IMG_CONGRATULATIONS,

		// モデル
		MODEL_PLAYER,
		MODEL_SKYDOME,
		MODEL_STAGE_BLANK,
		MODEL_STAGE_STONE,

		// 外部アニメーション

		// エフェクト

		// 映像
	};


	/*　各ハンドルパス　*/

	// エフェクトファイルパス
	static const std::string PATH_EFFECT;

	// フォントファイルパス
	static const std::string PATH_FONT;

	// 画像ファイルパス
	static const std::string PATH_IMAGE;

	// 3Dモデルファイルパス
	static const std::string PATH_MODEL;

	// アニメーションファイルパス
	static const std::string PATH_ANIM;

	// BGMファイルパス
	static const std::string PATH_BGM;

	// 効果音ファイルパス
	static const std::string PATH_SE;

	// 動画ファイルパス
	static const std::string PATH_MOVIE;

	// 動画ファイルパス
	static const std::string PATH_CSV;



	/// @brief インスタンス生成
	static void CreateInstance(void);

	/// @brief インスタンス取得処理
	static ResourceManager& GetInstance(void) { return *instance_; };

	/// @brief 初期化処理
	void Init(void);

	/// @brief インスタンス削除
	void Destroy(void);


	/// @brief リソースのロード
	/// @param _src 読み込み対象
	Resource Load(SRC _src);

	/// @brief リソースのハンドルを取得
	/// @param _src 読み込み対象
	const int LoadHandleId(SRC _src);
	const int& LoadHandleIds(SRC _src);



	/// @brief 3Dモデル重複利用時の読み込み
	/// @param src 読み込み対象
	int LoadModelDuplicate(SRC src);



private:

	//静的インスタンス
	static ResourceManager* instance_;

	// リソース管理対象
	std::unordered_map<SRC, Resource> resourcesMap_;

	// 読み込み済みリソース
	std::unordered_map<SRC, Resource*> loadedMap_;


	/// @brief 解放処理
	void Release(void);


	/// @brief デフォルトコンストラクタ
	ResourceManager(void);

	/// @brief デフォルトデストラクタ
	~ResourceManager(void) = default;

	// コピーコンストラクタ対策
	ResourceManager(const ResourceManager&) = delete;
	ResourceManager& operator=(const ResourceManager&) = delete;
	ResourceManager(ResourceManager&&) = delete;
	ResourceManager& operator=(ResourceManager&&) = delete;
	


	/// @brief リソースを取得する処理
	void SetResource(void);

	/// @brief リソース取得処理
	/// @param _loadType 読み込み方法
	/// @param _src リソース対象
	/// @param _path パス
	void _SetResource(Resource::LOAD_TYPE _loadType, SRC _src, std::string _path);

	/// @brief 内部ロード処理
	/// @param _source 読み込み対象
	Resource* _Load(SRC _source);
};