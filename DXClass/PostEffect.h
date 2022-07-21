#pragma once
#include "Sprite.h"
#include "Input.h"
#include "SafeDelete.h"

class PostEffect :
    public Sprite
{
private:// �ÓI�����o�ϐ�
    static const float clearColor[4];// ��ʃN���A�J���[
public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    PostEffect();
    ~PostEffect();

    /// <summary>
    /// ������
    /// </summary>
    void Init();

    /// <summary>
    /// �`��R�}���h�̔��s
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`��O
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void BeforeDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`���
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void AfterDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �p�C�v���C������
    /// </summary>
    void CreateGraphicsPipelineState();

    static float frag;

private:
    // �e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff[2];
    // SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    // �[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff;
    // RTV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    // DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;
    // �O���t�B�b�N�X�p�C�v���C��
    ComPtr<ID3D12PipelineState> pipelineState;
    // ���[�g�V�O�l�`��
    ComPtr<ID3D12RootSignature> rootSignature;
};

