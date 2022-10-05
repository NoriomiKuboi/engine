#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>
#include <d3dx12.h>

using namespace Microsoft::WRL;

class Audio
{
public:
	//�`�����N�w�b�_
	struct ChunkHeader
	{
		char id[4];//�`�����N����ID
		int	size;  // �`�����N�T�C�Y
	};
	//RIFF�w�b�_�`�����N
	struct RiffHeader
	{
		ChunkHeader chunk;//"RIFF"
		char type[4];//"WAVE"
	};
	//FMT�`�����N
	struct FormatChunk
	{
		ChunkHeader chunk;//"fmt"
		WAVEFORMATEX fmt;//�g�`�t�H�[�}�b�g
	};
	//�����f�[�^
	struct SoundData
	{
		WAVEFORMATEX wfex;//�g�`�t�H�[�}�b�g
		BYTE* pBuffer;//�o�b�t�@�̐擪�A�h���X
		unsigned int bufferSize;//�o�b�t�@�̃T�C�Y
		IXAudio2SourceVoice* pSourceVoice;
	};

private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	XAUDIO2_BUFFER buf{};

public:
	void Init(); // ����������
	SoundData SoundLoadWave(const char* fileName); //�����f�[�^�ǂݍ���
	void SoundUnload(SoundData* soundData); //�����f�[�^�̉��
	void CreateSoundData(SoundData& soundData); //�f�[�^�̐���
	bool SoundPlayWava(SoundData& soundData, bool roop); //�����Đ�
	void StopSound(SoundData& soundData); //�T�E���h�̒�~
};