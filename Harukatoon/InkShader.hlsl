

// カラーテクスチャ
Texture2D g_Texture : register(t0);
// テクスチャのノーマルマップ
Texture2D g_NormalMap : register(t1);
// テクスチャをどう描画するかのサンプラー
SamplerState g_Sampler : register(s0);

struct PS_INPUT
{
    float4 Pos      : SV_POSITION; // 画面上のピクセル位置
    float3 Normal   : NORMAL0;     // 頂点法線
    float2 TexCoord : TEXCOORD0;   // テクスチャのUV座標
};

float4 main(PS_INPUT Input) : SV_TARGET
{
    
	// テクスチャの色を読み込み
    float4 textureColor = g_Texture.Sample(g_Sampler, Input.TexCoord);
	
	// アルファ値がほぼゼロの場所は、描画をスキップする
    if (textureColor.a < 0.1f)
        discard;
    // ノーマルマップから法線をサンプリング
    float3 normalSample = g_NormalMap.Sample(g_Sampler, Input.TexCoord).rgb; 
    
    // 法線ベクトルを計算
    float3 normal;
    normal.xy = (normalSample.xy - 0.5f) * 2.0f; // 法線のXとYは中心からのオフセット
    normal.z = (normalSample.z - 0.5f) * 2.0f; // 法線のZは、オフセットの長さから計算
    normal.xy *= 1.5f;
    normal = normalize(normal); // 法線を正規化
    
    // 画面の右上から光を当てる
    float3 lightDir = normalize(float3(0.5f, -0.5f, -1.0f)); // 光の方向
    
    float NdotL = max(0.0f, dot(normal, -lightDir));
    
    float ambient = 0.5f; // 環境光の強さ
    float diffuse = NdotL; // 拡散成分の強さ
    
    float3 color = textureColor.rgb * (ambient + diffuse); // 環境光と拡散成分を組み合わせる
    
    
    // ツヤの計算
    float3 viewDir = normalize(float3(0.0f, 0.0f, -1.0f)); // 視線の方向
    float3 reflectDir = reflect(lightDir, normal); // 反射ベクトル
    // ツヤの広がり
    float specular = pow(max(0.0f,dot(viewDir, reflectDir)),20.0f); // ツヤの強さ
    
    //// フレネル効果(屈折光)
    float fresnel = pow(1.0f - dot(normal, viewDir), 1.0f); // フレネル効果の計算
    
    color += fresnel * 0.05f; // フレネル効果を色に加える
    
//    return textureColor; // テクスチャの色をそのまま返す
    
    return float4(color + specular, textureColor.a);
}