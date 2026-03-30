#include "Resource.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <string>


Resource::Resource(void):
	resType_(LOAD_TYPE::NONE), path_(""),
	allNum_(0), numX_(0), numY_(0),
	sizeX_(-1), sizeY_(-1),
	handleId_(-1)
{
}

Resource::Resource(LOAD_TYPE type, const std::string& path):
	resType_(type), path_(path),
	allNum_(0), numX_(0), numY_(0),
	sizeX_(-1), sizeY_(-1),
	handleId_(-1)
{
}

Resource::Resource(LOAD_TYPE type, const std::string& path
				  , int allNum, int numX, int numY, int sizeX, int sizeY):
	resType_(type), path_(path),
	allNum_(allNum), numX_(numX), numY_(numY),
	sizeX_(sizeX), sizeY_(sizeY),
	handleId_(-1)
{
	for (int i = 0; i < allNum_; i++)
	{
		handleIds_.emplace_back(-1);
	}
}



void Resource::Load(void)
{
	switch (resType_)
	{
		case LOAD_TYPE::IMAGE:
		case LOAD_TYPE::MOVIE:
		{
			handleId_ = LoadGraph(path_.c_str());
		}
		break;

		case LOAD_TYPE::IMAGES:
		{
			LoadDivGraph(path_.c_str(),
				         allNum_, numX_, numY_,
				         sizeX_, sizeY_,
				        (&handleIds_[0]));
		}
		break;

		case LOAD_TYPE::MODEL:
		case LOAD_TYPE::ANIM:
		{
			handleId_ = MV1LoadModel(path_.c_str());
		}
		break;
		
		
		case LOAD_TYPE::EFFECT:
		{
			handleId_ = LoadEffekseerEffect(path_.c_str());
		}
		break;

		case LOAD_TYPE::SOUND:
		{
			handleId_ = LoadSoundMem(path_.c_str());
		}
		break;
	}
}

void Resource::Release(void)
{
	switch (resType_)
	{
		// 単一画像
		case LOAD_TYPE::IMAGE:
		case LOAD_TYPE::MOVIE:
		{
			DeleteGraph(handleId_);
		}
		break;

		// 複数画像
		case LOAD_TYPE::IMAGES:
		{
			for (int i = 0; i < allNum_; i++)
			{
				DeleteGraph(handleIds_[i]);
			}
		}
		break;

		// 3Dモデル
		case LOAD_TYPE::MODEL:
		case LOAD_TYPE::ANIM:
		{
			MV1DeleteModel(handleId_);
			auto ids = duplicationModelIds_;
			for (auto id : ids)
			{
#if _DEBUG
				if (id == -1)
				{
					OutputDebugString("\nモデルが未割当です\n");
				}
#endif
				MV1DeleteModel(id);
			}
		}
		break;

		// エフェクト
		case LOAD_TYPE::EFFECT:
		{
			DeleteEffekseerEffect(handleId_);
		}
		break;

		case LOAD_TYPE::SOUND:
		{
			if (CheckSoundMem(handleId_) == 1)
			{
				// 再生時、再生中の音声を停止
				StopSoundMem(handleId_);
			}

			// 音声ファイルを解放
			DeleteSoundMem(handleId_);
		}
		break;
	}
}

void Resource::CopyHandle(int* images)
{
	for (int i = 0; i < allNum_; i++)
	{
		// ハンドル割り当て
		images[i] = handleIds_[i];
	}
}


void Resource::SetDuplicateModelId(int id)
{
	// IDをリストの末尾に追加
	duplicationModelIds_.push_back(id);
}