#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;

class Audio
{
public:
	//チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];//チャンク毎のID
		int	size;  // チャンクサイズ
	};
	//RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk;//"RIFF"
		char type[4];//"WAVE"
	};
	//FMTチャンク
	struct FormatChunk
	{
		ChunkHeader chunk;//"fmt"
		WAVEFORMATEX fmt;//波形フォーマット
	};
	//音声データ
	struct SoundData
	{
		WAVEFORMATEX wfex;//波形フォーマット
		BYTE* pBuffer;//バッファの先頭アドレス
		unsigned int bufferSize;//バッファのサイズ
		IXAudio2SourceVoice* pSourceVoice;
	};

private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	XAUDIO2_BUFFER buf{};

public:
	void Init(); // 初期化処理
	SoundData SoundLoadWave(const char* fileName); //音声データ読み込み
	void SoundUnload(SoundData* soundData); //音声データの解放
	void CreateSoundData(SoundData& soundData); //データの生成
	bool SoundPlayWava(SoundData& soundData, bool roop); //音声再生
	void StopSound(SoundData& soundData); //サウンドの停止
};