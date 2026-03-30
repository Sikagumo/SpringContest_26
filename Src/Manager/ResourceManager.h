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
		
		/* 単一画像 */
		IMG_TITLE,
		IMG_TITLE_BACK,
		IMG_SHADOW,

		/* 複数画像 */
		IMGS_TEXT,
		IMGS_TEXT_TIME,
		IMGS_UI_TITLE,
		IMGS_UI_SELECT,

		// モデル
		MODEL_PLAYER,
		MODEL_PLAYER_MOVE,
		MODEL_PLAYER_GRAVITY,

		MODEL_SKYDOME,
		MODEL_STAGE_BLANK,
		MODEL_STAGE_STONE,
		MODEL_GOAL,
		MODEL_TRAP,

		// 外部アニメーション

		// エフェクト

		// 映像
		
		/* BGM */
		BGM_TITLE,
		BGM_GAME,
		
		/* サウンドエフェクト */
		SE_CLICK,
		SE_SELECT,
		SE_CHANGE,
		SE_FANFALE,
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
	const int LoadHandleId(int _src) { return LoadHandleId(static_cast<SRC>(_src)); };

	/// @brief 複数画像のハンドルを画像ハンドルに割り当て
	/// @param _src 読み込み対象
	/// @param target 
	void LoadHandleIds(SRC _src, int* target);

	/// @brief 3Dモデル重複利用時の読み込み
	/// @param src 読み込み対象
	int LoadModelDuplicate(SRC src);

	/// @brief リソースのハンドルを取得
	/// @param _src 読み込み対象
	std::string GetHandlePath(SRC _src);
	std::string GetHandlePath(int _src) { return GetHandlePath(static_cast<SRC>(_src)); };


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

	/// @brief リソース取得処理(複数画像)
	/// @param _loadType 読み込み方法
	/// @param _src リソース対象
	/// @param _path パス
	/// @param _allNum 画像の数
	/// @param _numX 最大横画像数
	/// @param _numY 最大縦画像数
	/// @param _sizeX 各画像の横サイズ
	/// @param _sizeY 各画像の縦サイズ
	void _SetResource(Resource::LOAD_TYPE _loadType, SRC _src, std::string _path
					  , int _allNum = -1, int _numX = -1, int _numY = -1, int _sizeX = -1, int _sizeY = -1);

	/// @brief 内部ロード処理
	/// @param _source 読み込み対象
	Resource* _Load(SRC _source);
};