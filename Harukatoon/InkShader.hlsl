

// テクスチャ
Texture2D g_Texture : register(t0);
// テクスチャのノーマルマップ
Texture2D g_NormalMap : register(t1);
// テクスチャをどう描画するかのサンプラー
SamplerState g_Sampler : register(s0);

struct PS_INPUT
{
    float4 Pos      : SV_POSITION; // 頂点の位置
    float4 Color    : COLOR0;      // 頂点の色
    float2 TexCoord : TEXCOORD0;   // テクスチャのUV座標
};

float4 main(PS_INPUT Input) : SV_TARGET
{
	// テクスチャの色を読み込み
    float4 textureColor = g_Texture.Sample(g_Sampler, Input.TexCoord);
	
	// アルファ値がほぼゼロの場所は、描画をスキップする
    if (textureColor.a < 0.5f)discard;
    
    float3 normalSample = g_NormalMap.Sample(g_Sampler, Input.TexCoord).rgb; // ノーマルマップから法線をサンプリング
    
    // 法線ベクトルを計算
    float3 normal;
    normal.xy = (normalSample.xy - 0.5f) * 2.0f; // 法線のXとYは中心からのオフセット
    normal.z = (normalSample.z - 0.5f) * 2.0f;  // 法線のZは、オフセットの長さから計算
    normal = normalize(normal); // 法線を正規化
    
    // 画面の右上から光を当てる
    float3 lightDir = normalize(float3(0.5f, -0.5f, 1.0f)); // 光の方向
    
    // 光の色を計算
    float lightDiffuse = max(0.5f,dot(lightDir,normal)); // 光の拡散成分
    
    // ツヤの計算
    float3 viewDir = float3(0.0f, 0.0f, -1.0f); // 視線の方向
    float3 reflectDir = reflect(lightDir, normal); // 反射ベクトル
    // ツヤの広がり
    float specular = pow(max(0.0f,dot(viewDir, reflectDir)),30.0f)*0.7f; // ツヤの強さ
    
    // 最終的なインクの色の計算
    float3 shaderColor = (textureColor.rgb * lightDiffuse) + specular; // 光の影響を加える
    
    return float4(shaderColor,1.0f);
}