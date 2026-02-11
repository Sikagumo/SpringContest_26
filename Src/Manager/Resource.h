#pragma once
#include <string>
#include <vector>
#include <variant> // 候補型リスト

class Resource
{
public:

	enum class LOAD_TYPE
	{
		NONE,
		IMAGE,  // 単一画像
		IMAGES, // 複数画像
		MODEL,  // 3Dモデル
		MOVIE,  // 映像ファイル
		ANIM,   // アニメーションファイル
		EFFECT, // エフェクト
	};


	/// @brief デフォルトコンストラクタ
	Resource(void);

	/// @brief コンストラクタ
	/// @param _type 読み込むファイルの種類
	/// @param _path ファイルパス
	Resource(LOAD_TYPE _type, const std::string &_path);

	/// @brief 複数画像時のコンストラクタ
	/// @param _type 読み込むファイルの種類
	/// @param _path ファイルパス
	/// @param _numX 横画像数
	/// @param _numY 縦画像数
	/// @param _sizeX 画像１枚の横サイズ
	/// @param _sizeY 画像１枚の縦サイズ
	Resource(LOAD_TYPE _type, const std::string& _path, int _numX, int _numY, int _sizeX, int _sizeY);

	/// @brief デフォルトデストラクタ
	~Resource(void) = default;


	/// @brief 初回読み込み処理
	void Load(void);

	/// @brief 解放処理
	void Release(void);

	/// @brief ハンドルコピー処理
	/// @param _images 画像ハンドル
	void CopyHandle(int* _images);

	/// @brief 重複させる3DモデルID割り当て処理
	void SetDuplicateModelId(int _id);

	/// @brief ハンドルID取得処理
	int GetHandleId(void)const { return handleId_; };

	int* GetHandleIds(void)const { return handleIds_; };


private:

	// 読み込むファイルの種類
	LOAD_TYPE resType_;

	// 読み込み先
	std::string path_;
	
	// 単一読み込みハンドルID
	int handleId_;


	/* 複数画像読み込み用 */

	int* handleIds_; // 複数Idのポインタ
	int numX_;		 // 横画像数
	int numY_;		 // 縦画像数
	int sizeX_;		 // 横画像サイズ
	int sizeY_;		 // 縦画像サイズ

	// モデル複製用リスト
	std::vector<int> duplicationModelIds_;
};