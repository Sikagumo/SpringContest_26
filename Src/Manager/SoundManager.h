#pragma once
#include <map>
#include <string>
#include "Sound.h"
class ResourceManager;


class SoundManager
{
public:

	// ピッチの増減値
	static constexpr int PITCH_RANGE_DEF = 1;

	// 主音量最大
	static constexpr int VOLUME_MASTER_MAX = 255;

	// 主音量の(1/100)の値
	static constexpr float VOLUME_MASTER_NUM = (VOLUME_MASTER_MAX / 100);


	/* BGM音量 */

	// タイトルBGM音量
	static constexpr float VOLUME_TITLE = 0.9f;

	// ゲームシーンBGM音量
	static constexpr float VOLUME_GAME = 0.85f;


	/* BGM音量 */

	// クリックSEの音量
	static constexpr float VOLUME_CLICK = 0.9f;

	// 選択SEの音量
	static constexpr float VOLUME_SELECT = 0.9f;

	// 選択SEの音量
	static constexpr float VOLUME_CHANGE = 0.75f;

	// ファンファーレSEの音量
	static constexpr float VOLUME_FANFARE = 1.0f;

	static constexpr float VOLUME_COUNT_DOWN = 0.9f;

	static constexpr float VOLUME_DAMAGE = 1.0f;



	static void CreateInstance(void);
	static SoundManager& GetInstance(void) { return *instance_; };
	void DestroyInstance(void);

	void Load(void);


	/// @brief 2Dサウンド再生処理
	/// @param _src 再生するサウンドの種類
	/// @param _isLoop ループするか否か
	/// @param _isPitch ピッチを変えるか否か
	/// @param _pitchRange ピッチ増減の範囲
	/// @param _isForce 
	bool Play(int _src, bool _isLoop, bool _isPitch = false, int _pitchRange = PITCH_RANGE_DEF, bool _isForce = false);

	/// @brief 3Dサウンド再生処理
	/// @param _sec 再生するサウンドの種類
	/// @param _isLoop ループするか否か
	/// @param _pos 音量が出てくる座標
	/// @param _radius 音量の聞こえる半径
	/// @returns 再生しているか否か
	bool Play(int _sec, bool _isLoop, VECTOR _pos, float _radius);


	/// @brief サウンドが再生開始しているか判定
	/// @param _src 再生するサウンドの種類
	bool IsSoundStart(int _src);

	/// @brief サウンドが再生しているか判定
	/// @param _src 再生するサウンドの種類
	bool IsSoundPlay(int _src);

	/// @brief サウンドが停止しているか判定
	/// @param _src 再生するサウンドの種類
	bool IsSoundEnd(int _src);

	/// @brief 指定の音声を停止
	void Stop(int _src);

	/// @brief 全音声を停止
	void StopAll(void);

	/// @brief 指定の種類の音声を停止
	/// @param _isBGM (true:BGMを停止, false:SEを停止)
	void StopAllChoice(bool _isBGM);

	/// @brief 音量を割り当て
	/// @param _src 割り当てるサウンド
	/// @param _per 音量の倍率(0.0～1.0)
	void SetVolume(int _src, float _per = 1.0f);

	/// @brief 指定の音声の音量を取得
	/// @param _src 指定するサウンドの種類
	/// @return 音量の割合(0.0～1.0)
	float GetVolume(int _src);

	/// @brief 主音量割り当て
	/// @param _volumeRate 指定する割合(0.0～1.0)
	void SetVolumeMaster(float _volumeRate = 1.0f);

	/// @brief 主音量取得
	int GetVolumeMaster(void)const { return volumeMaster_; };



private:

	ResourceManager& resMng_;


	// 静的インスタンス
	static SoundManager* instance_;

	// 音声リスト
	std::map<int, Sound*> sounds_;

	// 主音量
	int volumeMaster_;


	SoundManager(void);
	~SoundManager(void) = default;

	/* コピーコンストラクタ対策 */
	SoundManager(const SoundManager&)			 = delete;
	SoundManager& operator=(const SoundManager&) = delete;
	SoundManager(SoundManager&&)				 = delete;
	SoundManager& operator=(SoundManager&&)		 = delete;

	/// @brief 各音声割り当て処理
	void SetSounds(void);

	/// @brief 音声割り当て処理
	/// @param _src リソース
	/// @param type 再生タイプ
	/// @param maxVolume 最大音量
	void SetSound(int _src, Sound::TYPE type, float maxVolume, bool _isBGM = false);
};