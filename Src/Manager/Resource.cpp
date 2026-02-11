#include "Resource.h"
#include <DxLib.h>
#include <EffekseerForDXLib.h>
#include <string>


Resource::Resource(void):
	resType_(LOAD_TYPE::NONE), path_(""),
	numX_(-1), numY_(-1),
	sizeX_(-1), sizeY_(-1),
	handleId_(-1), handleIds_(nullptr)
{
}

Resource::Resource(LOAD_TYPE type, const std::string& path):
	resType_(type), path_(path),
	numX_(-1), numY_(-1),
	sizeX_(-1), sizeY_(-1),
	handleId_(-1), handleIds_(nullptr)
{
}

Resource::Resource(LOAD_TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY):
	resType_(type), path_(path),
	numX_(numX), numY_(numY),
	sizeX_(sizeX), sizeY_(sizeY),
	handleId_(-1), handleIds_(nullptr)
{
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
			handleIds_ = new int[numX_ * numY_];
			LoadDivGraph(path_.c_str(),
				        (numX_ * numY_), numX_, numY_,
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
			int max = (numX_ * numY_);
			for (int i = 0; i < max; i++)
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
	}
}

void Resource::CopyHandle(int* images)
{
	// 複数IDがない時、処理終了
	if (handleIds_ == nullptr) return;

	int max = (numX_ * numY_);
	for (int i = 0; i < max; i++)
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