#include "PostEffectTestSH.hlsli"

Texture2D<float4> tex : register(t0);  // 0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv);
	//float4 texcolor = tex.Sample(smp, input.uv + float2(0.6f, 0.0f));

	return float4(texcolor.rgb, 1);
	//return float4(texcolor.rgb * 2.0f, 1);
}