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
	void Init(WindowApp* win); // 初期化
	void BeforeDraw(); // 描画前処理
	void AfterDraw(); // 描画後処理
	void ClearRenderTarget(); // レンダーターゲットのクリア
	void ClearDepthBuffer(); // 深度バッファのクリア

	ID3D12Device* GetDev() { return dev.Get(); } // デバイスの取得
	ID3D12GraphicsCommandList* GetCmdList() { return cmdList.Get(); }	// 描画コマンドリストの取得

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
	bool InitDXGIDevice(); // DXGIデバイス初期化
	bool CreateSwapChain(); // スワップチェーンの生成
	bool InitCommand(); // コマンド関連初期化
	bool CreateFinalRenderTargets(); // レンダーターゲット生成
	bool CreateDepthBuffer(); // 深度バッファ生成
	bool CreateFence(); // フェンス生成
};