#include "ImguiManager.h"

ImguiManager::ImguiManager(HWND _hwnd, ID3D12Device* _dev, ID3D12GraphicsCommandList* _cmdList)
{
	hwnd = _hwnd;
	dev = _dev;
	cmdList = _cmdList;

	D3D12_DESCRIPTOR_HEAP_DESC desc = {};
	desc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	desc.NodeMask = 0;
	desc.NumDescriptors = 1;
	desc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	dev->CreateDescriptorHeap(&desc, IID_PPV_ARGS(heapForImgui.ReleaseAndGetAddressOf()));

	if (ImGui::CreateContext() == nullptr)
	{
		assert(0);
	}

	bool blnResult = ImGui_ImplWin32_Init(hwnd);
	if (!blnResult)
	{
		assert(0);
	}

	blnResult = ImGui_ImplDX12_Init(
		dev,
		1,
		DXGI_FORMAT_R8G8B8A8_UNORM,
		heapForImgui.Get(),
		heapForImgui.Get()->GetCPUDescriptorHandleForHeapStart(),
		heapForImgui.Get()->GetGPUDescriptorHandleForHeapStart());
}

void ImguiManager::cmdExecute()
{
	ImGui::Render();

	cmdList->SetDescriptorHeaps(
		1,
		heapForImgui.GetAddressOf());
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), cmdList);
}
