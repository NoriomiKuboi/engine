//　ボーンの最大数
static const int MAX_BONES = 32;

cbuffer skinning : register(b3)// ボーンのスキニング行列が入る
{
	matrix matSkinning[MAX_BONES];
}

cbuffer cbuff0 : register(b0)
{
	matrix viewproj;// ビュープロジェクション行列
	matrix world;// ワールド行列
	float3 cameraPos;// カメラ座標(ワールド座標)
}

// バーテックスバッファーの入力
struct VSInput
{
	float4 pos : POSITION;// 位置
	float3 normal : NORMAL;// 頂点座標
	float2 uv : TEXCOORD;// テクスチャ―座標
	uint4 boneIndices : BONEINDICES;// ボーン番号
	float4 boneWeights : BONEWEIGHTS;// ボーンのスキンウェイト
};

// 頂点シェーダーからピクセルシェーダーへのやり取りに使用する
struct VSOutput
{
	float4 svpos : SV_POSITION;// システム用頂点座標
	float3 normal : NORMAL;// 法線
	float2 uv : TEXCOORD;// uv値
};