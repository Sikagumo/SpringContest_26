#include "ResourceManager.h"
#include "Resource.h"
#include <DxLib.h>
#include <string>
#include <unordered_map>
#include "../Application.h"
#include "../CSV/CsvManager.h"
#include "../CSV/StatusPlayer.h"
#include "../CSV/StatusEnemy.h"

ResourceManager* ResourceManager::instance_ = nullptr;

// リソースファイルのパス
#ifdef _DEBUG
const std::string PATH_DATA = "Data/";

// 暗号化済みのリソースフォルダパス
#else

const std::string PATH_DATA = "_ResourceData/";
#endif


// ファイルパスの割り当て
const std::string ResourceManager::PATH_EFFECT = PATH_DATA + "Effect/";
const std::string ResourceManager::PATH_FONT   = PATH_DATA + "Font/";
const std::string ResourceManager::PATH_IMAGE  = PATH_DATA + "Image/";
const std::string ResourceManager::PATH_MODEL  = PATH_DATA + "Model/";
const std::string ResourceManager::PATH_ANIM   = PATH_DATA + "Model/Animation/";
const std::string ResourceManager::PATH_SE     = PATH_DATA + "Sound/SE/";
const std::string ResourceManager::PATH_BGM    = PATH_DATA + "Sound/BGM/";
const std::string ResourceManager::PATH_MOVIE  = PATH_DATA + "Movie/";
const std::string ResourceManager::PATH_CSV  = PATH_DATA + "CSV/";


void ResourceManager::CreateInstance(void)
{
	if (instance_ == nullptr)
	{
		instance_ = new ResourceManager();
	}

	instance_->Init();
}

ResourceManager::ResourceManager(void)
{
	
}


void ResourceManager::Init(void)
{
	SetResource(); // リソース取得
}

void ResourceManager::SetResource(void)
{
	using LOAD_TYPE = Resource::LOAD_TYPE;

	_SetResource(LOAD_TYPE::MODEL, SRC::MODEL_PLAYER, PATH_MODEL + "Player/Player.mv1");
	_SetResource(LOAD_TYPE::MODEL, SRC::MODEL_SKYDOME, PATH_MODEL + "SkyDome/SkyDome.mv1");
}
void ResourceManager::_SetResource(Resource::LOAD_TYPE _loadType, SRC _src, std::string _path)
{
	Resource res;
	res = Resource(_loadType, _path);
	resourcesMap_.emplace(_src, res);
}


void ResourceManager::Release(void)
{
	/* メモリ解放処理 */

	if (!resourcesMap_.empty())
	{
		// リソースリストをクリア(空の時は行わない)
		resourcesMap_.clear();
	}
	if (!loadedMap_.empty())
	{
		for (auto& [src, resource] : loadedMap_)
		{
			// 読み込み済みリソース解放
			resource->Release();
			delete resource;
		}

		// 読み込み済みリソースリストをクリア
		loadedMap_.clear();
	}
}

void ResourceManager::Destroy(void)
{
	/*　インスタンス削除処理　*/

	instance_->Release();
	delete instance_;
}


Resource ResourceManager::Load(SRC src)
{
	// 読み込み処理
	Resource* res = _Load(src);

	if (res == nullptr) return Resource();

	Resource ret = *res;

	return *res;
}
const int ResourceManager::LoadHandleId(SRC _src)
{
	return Load(_src).GetHandleId();
}
const int& ResourceManager::LoadHandleIds(SRC _src)
{
	return *Load(_src).GetHandleIds();
}

Resource* ResourceManager::_Load(SRC src)
{
	// 読み込み済みリストを検索
	const auto& loaded = loadedMap_.find(src);

	//読み込み済みリストに対象がある時、要素を返す
	if (loaded != loadedMap_.end()) return loaded->second;


	// リソースリスト内を検索
	const auto& resource = resourcesMap_.find(src);

	// リソースリストに登録されてない時、NULLを返す
	if (resource == resourcesMap_.end()) return nullptr;


	// リソースリスト登録済み時、読み込み処理
	resource->second.Load();

	// 念のためにコピーコンストラクタ
	Resource* ret = new Resource(resource->second);

	// 読み込み済みリストに格納
	loadedMap_.emplace(src, ret);

	return ret;
}


int ResourceManager::LoadModelDuplicate(SRC src)
{
	/* 3Dモデル重複利用時の読み込み */

	// 読み込み処理
	Resource* resource = _Load(src);

	// 読み込み失敗
	if (resource == nullptr)
	{
		return -1;
	}

	// 重複するモデルのハンドルを取得
	int id = MV1DuplicateModel(resource->GetHandleId());

	// 重複モデルリストにハンドル追加
	resource->SetDuplicateModelId(id);

	return id;
}