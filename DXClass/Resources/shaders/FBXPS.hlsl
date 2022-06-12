#include "FBX.hlsli"

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
};

//0 番スロットに設定されたテクスチャ
Texture2D<float4> tex : register(t0);
// 0番スロットに設定されたサンプラー
SamplerState smp : register(s0);
// エントリーポイント

/*float4 main(VSOutput input) : SV_TARGET
{
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp,input.uv);
	// Lambert反射
	float3 light = normalize(float3(1, -1, 1));// 右下奥
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadercolor = float4(brightness, brightness, brightness, 1.0f);
	// 陰影とテクスチャの色を合成
	return shadercolor * texcolor;
}*/

PSOutput main(VSOutput input)
{
	PSOutput output;
	// テクスチャマッピング
	float4 texcolor = tex.Sample(smp,input.uv);
	// Lambert反射
	float3 light = normalize(float3(1, -1, 1));// 右下奥
	float diffuse = saturate(dot(-light, input.normal));
	float brightness = diffuse + 0.3f;
	float4 shadercolor = float4(brightness, brightness, brightness, 1.0f);
	// 陰影とテクスチャの色を合成
	output.target0 = shadercolor * texcolor;
	output.target1 = float4(1 - (shadercolor * texcolor).rgb, 1);
	return output;
}