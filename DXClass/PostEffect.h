#pragma once
#include "Sprite.h"

class PostEffect :
    public Sprite
{
private:// 静的メンバ変数
    static const float clearColor[4];// 画面クリアカラー

public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    PostEffect();

    /// <summary>
    /// 初期化
    /// </summary>
    void Init();

    /// <summary>
    /// 描画コマンドの発行
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画前
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void BeforeDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画後
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void AfterDrawScene(ID3D12GraphicsCommandList* cmdList);

private:
    // テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;
    // SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    // 深度バッファ
    ComPtr<ID3D12Resource> depthBuff;
    // RTV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    // DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;
};

