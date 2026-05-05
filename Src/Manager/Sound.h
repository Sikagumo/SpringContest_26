#pragma once
#include <string>
#include <DxLib.h>
#include "../Utility/UtilityMath.h"

class Sound
{
public:

	static constexpr int VOLUME_MAX = 255;
	
	enum class TYPE
	{
		NONE = -1,
		SOUND_2D, // 2D音響
		SOUND_3D, // 3D音響
	};

	enum class TIMES
	{
		NONE = -1,
		ONCE, // １回
		LOOP, // 繰り返し
	};


	Sound(void);

	/// @brief コンストラクタ
	/// @param _type 音声の方法種類(2D or 3D)
	/// @param _src 音声リソースの種類
	/// @param _handle 音声リソースのハンドル
	/// @param _isBGM BGMか否か
	Sound(TYPE _type, int _src, int _handle, bool _isBGM);

	~Sound(void) = default;


	/// @brief 更新処理
	/// @param _pos 3D座標更新処理
	void Update(VECTOR _pos);

	/// @brief 読み込み処理
	void Load(void)const;


	/// @brief 2Dサウンドの再生処理
	/// @param _times 再生種類を取得
	/// @param _isPitch ピッチを上げるか否か
	/// @param _pitchRange ピッチの増加範囲
	/// @param _isForce
	/// @returns 音声未割当時、false
	bool Play(TIMES _times, bool _isPitch = false, int _pitchRange = 1, bool _isForce = false);

	/// @brief 3Dサウンドの再生処理
	/// @param times 再生種類を取得
	/// @param pos 音声が出てくる座標
	/// @param radius 音声が聞こえる範囲
	bool Play(TIMES _times, VECTOR _pos, float _radius);

	/// @brief 停止処理
	void Stop(void);

	/// @brief サウンドがあるか判定
	bool CheckSoundHandle(void);

	/// @brief 音声が読み込まれているか否か
	bool IsLoad(void)const { return (sound_.handle != -1); };


	/// @brief 現在の音量を割り当て
	/// @param per 音量(0.0～1.0)
	void SetVolume(float _volume = 1.0f)const;

	/// @brief 現在の音量を取得
	/// @return 音量の割合(0.0～1.0)
	float GetVolume(void)const;

	/// @brief 最大音量を割り当て
	/// @param _volume 最大音量(0.0～1.0)
	void SetMaxVolume(float _volume = 1.0f);


	/// @brief 再生を開始したか判定
	bool IsStart(void)const;

	/// @brief 再生を開始したか判定
	bool IsPlay(void)const;

	/// @brief 再生が終了したか判定
	bool IsEnd(void)const;

	/// @brief BGMか判定
	bool IsBGM(void)const;


private:

	// 半音階(1オクターブ=1200.0f)
	static constexpr float MUSICAL_SCALE_HALF = (100.0f / 2.0f);

	struct SoundInfo
	{
		// 音声リソースの種類
		int src;

		// ハンドルID
		int handle;

		// 再生する種類(2D or 3D)
		TYPE type;

		// 最大音声
		int maxVolume;

		// 再生位置(3D用)
		VECTOR pos;

		// 有効範囲(3D用)
		float radius;

		// 再生しているか否か
		bool isPlay;

		// 再生しているか否か
		bool isPlayOld;

		// 再生しているか否か
		bool isBgm;

		SoundInfo(void)
			: type(TYPE::NONE)
			, src(-1), handle(-1)
			, maxVolume(VOLUME_MAX)
			, radius(0.0f), pos(UtilityMath::VECTOR_ZERO)
			, isPlay(false), isPlayOld(false)
			, isBgm(false)
		{
		};

		SoundInfo(TYPE _type, int _src, int _handle, bool _isBGM)
			: type(_type)
			, src(_src), handle(_handle)
			, maxVolume(VOLUME_MAX)
			, radius(0.0f), pos(UtilityMath::VECTOR_ZERO)
			, isPlay(false), isPlayOld(false)
			, isBgm(_isBGM)
		{
		};
	};

	SoundInfo sound_;
};