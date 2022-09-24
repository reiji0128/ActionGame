#pragma once

class Shader;

class GBuffer
{
public:
	GBuffer();

	~GBuffer();

	/// <summary>
	/// G-Bufferレンダリングの開始
	/// </summary>
	void GBufferRenderingBegin();


	/// <summary>
	/// G-Bufferレンダリングの終了
	/// </summary>
	void GBufferRenderingEnd();

	/// <summary>
	/// G-Bufferの作成
	/// </summary>
	void CreateGBuffer();

	/// <summary>
	/// G-Bufferをシェーダーに入力に渡す
	/// </summary>
	void InputGBufferToShader();

private:
	int mScreenWidth;

	int mScreenHeight;

	// G-BufferのID
	unsigned int mGBuffer;

	// 座標テクスチャのID
	unsigned int mGPosition;

	// 法線テクスチャのID
	unsigned int mGNormal;

	// アルベドとスペキュラテクスチャのID
	unsigned int mGAlbedoSpec;
};