#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>

#include "WindowApp.h"

class DirectXCommon
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	void Init(WindowApp* win); // ������
	void BeforeDraw(); // �`��O����
	void AfterDraw(); // �`��㏈��
	void ClearRenderTarget(); // �����_�[�^�[�Q�b�g�̃N���A
	void ClearDepthBuffer(); // �[�x�o�b�t�@�̃N���A

	ID3D12Device* GetDev() { return dev.Get(); } // �f�o�C�X�̎擾
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }	// �`��R�}���h���X�g�̎擾

private:
	WindowApp* winApp;

	ComPtr<IDXGIFactory6> dxgiFactory;
	ComPtr<ID3D12Device> dev;
	ComPtr<ID3D12GraphicsCommandList> cmdList;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12CommandQueue> cmdQueue;
	ComPtr<IDXGISwapChain4> swapchain;
	std::vector<ComPtr<ID3D12Resource>> backBuffers;
	ComPtr<ID3D12Resource> depthBuffer;
	ComPtr<ID3D12DescriptorHeap> rtvHeaps;
	ComPtr<ID3D12DescriptorHeap> dsvHeap;
	ComPtr<ID3D12Fence> fence;
	UINT64 fenceVal = 0;

private:
	bool InitDXGIDevice(); // DXGI�f�o�C�X������
	bool CreateSwapChain(); // �X���b�v�`�F�[���̐���
	bool InitCommand(); // �R�}���h�֘A������
	bool CreateFinalRenderTargets(); // �����_�[�^�[�Q�b�g����
	bool CreateDepthBuffer(); // �[�x�o�b�t�@����
	bool CreateFence(); // �t�F���X����
};