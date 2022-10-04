#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <wrl.h>
#include <d3dx12.h>
#include <cstdlib>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_win32.h"
#include "imgui/imgui_impl_dx12.h"

class ImguiManager
{
private:
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	ImguiManager(HWND _hwnd, ID3D12Device* _dev, ID3D12GraphicsCommandList* _cmdList);
	~ImguiManager();

	void cmdExecute();

	// ÉqÅ[Évï€éù
	ComPtr<ID3D12DescriptorHeap> heapForImgui;

	HWND hwnd;
	ID3D12Device* dev;
	ID3D12GraphicsCommandList* cmdList;
};

