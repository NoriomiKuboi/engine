#include "Audio.h"
#include <fstream>
#include <cassert>

#pragma comment(lib,"xaudio2.lib")

using namespace std;


void Audio::Init()
{
	HRESULT result = S_FALSE;

	//XAudio�G���W���̃C���X�^���X�𐶐�
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);

	//�}�X�^�[�{�C�X�𐶐�
	result = xAudio2->CreateMasteringVoice(&masterVoice);
}

//�����f�[�^�ǂݍ���
Audio::SoundData Audio::SoundLoadWave(const char* fileName) 
{
	SoundData soundData;

	//�t�@�C�����̓X�g���[���̃C���X�^���X
	ifstream file;

	//wav�t�@�C�����o�C�i�����[�h�ŊJ��
	file.open(fileName, ios_base::binary);

	//�t�@�C���I�[�v�����s�����m
	assert(file.is_open());

	///wav�f�[�^�ǂݍ���
	//RIFF�w�b�_�[�̓ǂݍ���
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));

	//�t�@�C����RIFF���`�F�b�N
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0)
	{
		assert(0);
	}

	//�^�C�v��WAVE���`�F�b�N
	if (strncmp(riff.type, "WAVE", 4) != 0)
	{
		assert(0);
	}

	//Format�`�����N�̓ǂݍ���
	FormatChunk format = {};

	//�`�����N�w�b�_�[�̊m�F
	file.read((char*)&format, sizeof(ChunkHeader));

	//�`�����N�{�̂̓ǂݍ���
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char*)&format.fmt, format.chunk.size);

	//Data�`�����N�̓ǂݍ���
	ChunkHeader data;
	file.read((char*)&data, sizeof(data));

	//JUNK�`�����N�����o�����ꍇ
	if (strncmp(data.id, "JUNK ", 4) == 0 ||
		strncmp(data.id, "LIST", 4) == 0)
	{
		//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
		file.seekg(data.size, ios_base::cur);

		//�ēx�ǂݍ���
		file.read((char*)&data, sizeof(data));
	}

	if (strncmp(data.id, "data", 4) != 0)
	{
		assert(0);
	}

	//Data�`�����N�̃f�[�^��(�g�`�f�[�^)�̓ǂݍ���
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//Wave�t�@�C�������
	file.close();

	///�ǂݍ��񂾉����f�[�^��Ԃ�
	//return���邽�߂̉����f�[�^
	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE*>(pBuffer);
	soundData.bufferSize = data.size;

	CreateSoundData(soundData);
	float SourceVoiceChannelVolumes[1] = { 0.0 };
	soundData.pSourceVoice->SetChannelVolumes(1, SourceVoiceChannelVolumes);

	return soundData;
}

//�����f�[�^�̉��
void Audio::SoundUnload(SoundData* soundData) 
{
	//�o�b�t�@�̃����������
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

//�f�[�^�̐���
void Audio::CreateSoundData(SoundData& soundData) 
{
	HRESULT result;

	//�g�`�t�H�[�}�b�g������SourceVoice�̐���
	result = xAudio2->CreateSourceVoice(&soundData.pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

}

//�����Đ�
bool Audio::SoundPlayWava(SoundData& soundData, bool roop) 
{
	HRESULT result;

	XAUDIO2_VOICE_STATE xa3state;
	soundData.pSourceVoice->GetState(&xa3state);
	//0�łȂ��Ȃ特�y���Đ����ɂȂ邽�߃X���[����
	if (xa3state.BuffersQueued != 0)
	{
		return false;
	}

	//�f�[�^����
	CreateSoundData(soundData);

	//�Đ�����g�`�f�[�^�̐ݒ�
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (roop == true)
	{
		buf.LoopCount = XAUDIO2_LOOP_INFINITE;
	}

	//�g�`�f�[�^�̍Đ�
	result = soundData.pSourceVoice->SubmitSourceBuffer(&buf);
	result = soundData.pSourceVoice->Start();

	buf = { NULL };

	return true;
}

//���y�̒�~
void Audio::StopSound(SoundData& soundData)
{
	soundData.pSourceVoice->Stop(0);
	soundData.pSourceVoice->FlushSourceBuffers();
	soundData.pSourceVoice->SubmitSourceBuffer(&buf);
}