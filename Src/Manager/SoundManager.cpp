#include "SoundManager.h"
#include <DxLib.h>
#include <string>
#include <cassert>
#include "./ResourceManager.h"
#include "./Resource.h"


SoundManager* SoundManager::instance_ = nullptr;

void SoundManager::CreateInstance(void)
{
	/*　インスタンス初期化処理　*/
	if (instance_ == nullptr)
	{
		instance_ = new SoundManager();
	}

	// 初期化処理
	instance_->Load();
}
SoundManager::SoundManager(void):
	resMng_(ResourceManager::GetInstance()),
	volumeMaster_(VOLUME_MASTER_MAX)
{
}


void SoundManager::Load(void)
{
	// リストクリア
	sounds_.clear();

	// 各音声割り当て
	SetSounds();
}
void SoundManager::SetSounds(void)
{
	int src = -1;

	/* サウンドエフェクト */
	//src = static_cast<int>(ResourceManager::SRC::SE_CLICK);
	//SetSound(src, Sound::TYPE::SOUND_2D, VOLUME_CLICK);


	/* BGM */

	// タイトルシーン
	src = static_cast<int>(ResourceManager::SRC::BGM_TITLE);
	SetSound(src, Sound::TYPE::SOUND_2D, VOLUME_TITLE, true);

	// ゲームシーン
	src = static_cast<int>(ResourceManager::SRC::BGM_GAME);
	SetSound(src, Sound::TYPE::SOUND_2D, VOLUME_GAME, true);

}
void SoundManager::SetSound(int _src, Sound::TYPE type, float maxVolume, bool _isBGM)
{
	/* 音声登録 */
	Sound* res = new Sound(type, _src,
						   ResourceManager::GetInstance().LoadHandleId(_src),
						   _isBGM);

	res->SetMaxVolume(maxVolume);

	sounds_.emplace(_src, res);
}

void SoundManager::Destroy(void)
{
	/*　メモリ解放処理　*/
	for (auto& [type, sound] : sounds_)
	{
		delete sound;
	}

	// リストクリア
	sounds_.clear();

	delete instance_;
}

bool SoundManager::Play(int _src, bool _isLoop, bool _isPitch, float _pitchRange, bool _isForce)
{
	/* 2D音声を再生 */
	const auto& findSound = sounds_.find(_src);

	if (findSound == sounds_.end())
	{
#ifdef _DEBUG
		assert(false && "\n2D再生する音声が割り当てられていません\n(；_；)\n"); // 例外スロー
#endif
		return false;
	}


	if (!findSound->second->IsLoad())
	{
		// 読み込み処理
		findSound->second->Load();
	}

	// 2Dサウンド再生処理
	Sound::TIMES times = ((_isLoop) ? Sound::TIMES::LOOP : Sound::TIMES::ONCE);
	return findSound->second->Play(times, _isPitch, _pitchRange,_isForce);
}
bool SoundManager::Play(int _src, bool _isLoop, VECTOR _pos, float _radius)
{
	/* 3D音声を再生 */
	const auto& lPair = sounds_.find(_src);

	if (lPair == sounds_.end())
	{
#ifdef _DEBUG
		assert(false && "\n3D再生する音声が割り当てられていません\n(；_；)\n"); // 例外スロー
#endif
		return false;
	}


	if (!lPair->second->IsLoad())
	{
		// 読み込み処理
		lPair->second->Load();
	}

	// 3Dサウンド再生処理
	Sound::TIMES times = ((_isLoop) ? Sound::TIMES::LOOP : Sound::TIMES::ONCE);
	return lPair->second->Play(times, _pos, _radius);
}

void SoundManager::Stop(int _src)
{
	const auto& listPair = sounds_.find(_src);

	// 判定対象
	if (listPair == sounds_.end())
	{
#ifdef _DEBUG
		OutputDebugString("\n停止する音声が割り当てられていません\n(；_；)\n");
		assert(false); // 例外スロー
#endif
		return;
	}
	
	// 指定の音声を停止
	return listPair->second->Stop();
}

void SoundManager::StopAll(void)
{
	if (sounds_.empty()) return;

	for (auto& sound : sounds_)
	{
		if (!sound.second->IsLoad())
		{
			// 未読み込み時、スキップ
			continue;
		}

		// リスト内のサウンドを全て停止
		sound.second->Stop();
	}
}

void SoundManager::StopAllChoice(bool _isBGM)
{
	/* 指定した種類の音声を停止 */

	if (sounds_.empty()) return;

	for (auto& sound : sounds_)
	{
		if (!sound.second->IsLoad() ||
			!sound.second->IsBGM() == _isBGM)
		{
			// 未読み込み時、指定の種類以外スキップ
			continue;
		}

		// リスト内のサウンドを全て停止
		sound.second->Stop();
	}
}

void SoundManager::SetVolume(int _src, float _per)
{
	const auto& lPair = sounds_.find(_src);

	if (lPair != sounds_.end())
	{
		// 音声があるとき、音量を割り当て
		return lPair->second->SetVolume(_per);
	}

#ifdef _DEBUG
	OutputDebugString("\nサウンドが割り当てられていません\n(；_；)\n");
	assert(false); // 例外スロー
#endif
}

bool SoundManager::IsSoundStart(int _src)
{
	const auto& soundlist = sounds_.find(_src);

	if (soundlist == sounds_.end())
	{
#ifdef _DEBUG
		assert(false &&"\n停止するサウンドが割り当てられていません\n(；_；)\n"); // 例外スロー
#endif
		return false;
	}

	// 音声があるとき、再生か判定
	return soundlist->second->IsStart();
}

bool SoundManager::IsSoundPlay(int _src)
{
	const auto& soundlist = sounds_.find(_src);

	if (soundlist == sounds_.end())
	{
#ifdef _DEBUG
		assert(false &&"\n停止するサウンドが割り当てられていません\n(；_；)\n"); // 例外スロー
#endif
		return false;
	}

	// 音声があるとき、再生か判定
	return soundlist->second->IsPlay();
}

bool SoundManager::IsSoundEnd(int _src)
{
	const auto& soundlist = sounds_.find(_src);

	if (soundlist == sounds_.end())
	{
#ifdef _DEBUG
		assert(false &&"\n停止するサウンドが割り当てられていません\n(；_；)\n"); // 例外スロー
#endif
		return false;
	}

	// 音声があるとき、再生か判定

	return soundlist->second->IsEnd();
}

void SoundManager::SetVolumeMaster(float volume)
{
	float vol = volume;
	
	// 音量が０未満の時、音量を０にする
	vol = ((vol < 0.0f) ? 0.0f : vol);

	// 音量が最大値を超えた時、音量を最大値にする
	vol = ((volume > VOLUME_MASTER_MAX) ? VOLUME_MASTER_MAX : vol);

	vol = static_cast<int>(volume * VOLUME_MASTER_MAX);

	// 音量割り当て
	volumeMaster_ = vol;
}