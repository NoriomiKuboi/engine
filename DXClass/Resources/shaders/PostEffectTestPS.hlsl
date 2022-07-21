#include "PostEffectTestSH.hlsli"

Texture2D<float4> tex0 : register(t0);  // 0番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);  // 1番スロットに設定されたテクスチャ
SamplerState smp : register(s0);      // 0番スロットに設定されたサンプラー

/*float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor = tex.Sample(smp, input.uv);
	//float4 texcolor = tex.Sample(smp, input.uv + float2(0.6f, 0.0f));

	return float4(texcolor.rgb, 1);
	//return float4(texcolor.rgb * 2.0f, 1);
}*/

float4 main(VSOutput input) : SV_TARGET
{
	float4 texcolor0 = tex0.Sample(smp, input.uv);

	if (change == 1.0f)
	{
		float4 _color0 = texcolor0;
		float grayscale = _color0.r * 0.299 + _color0.g * 0.587 + _color0.b * 0.114;

		// グレイスケール
		return float4(grayscale, grayscale, grayscale, 1);
	}

	if (change == 2.0f)
	{
		float2 uv = 2 * input.uv - 1;

		float r = length(uv);
		r = 0.7 * r - 0.7;

		float a = atan2(uv.y, uv.x);
		a = abs(cos(50 * a) + sin(20 * a));

		float d = a - r;
		float n = smoothstep(0.1, 0.4, saturate(d));

		float4 _color1 = tex0.Sample(smp, input.uv);

		// 集中線
		return n * _color1;
	}

	if (change == 3.0f)
	{
		float2 uv = input.uv;
		float4 _color2 = tex0.Sample(smp, input.uv);

		uv = 2. * uv - 1.;
		_color2 *= 1.0 - dot(uv, uv);

		// ヴィネット
		return _color2;
	}

	if (change == 4.0f)
	{
		// 単色シェーダー
		return float4 (1, 1, 0, 1);
	}

	// 色反転
	return float4 (1.0f - texcolor0.r, 1.0f - texcolor0.g, 1.0f - texcolor0.b, 1.0f);

}