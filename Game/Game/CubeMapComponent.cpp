#include "CubeMapComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "GameObject.h"
#include "Texture.h"
#include "Shader.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="owner">ゲームオブジェクトのポインタ</param>
CubeMapComponent::CubeMapComponent(GameObject* owner)
	:Component(owner)
	,mVisible(true)
{
}

/// <summary>
/// デストラクタ
/// </summary>
CubeMapComponent::~CubeMapComponent()
{
}

/// <summary>
/// テクスチャの作成
/// </summary>
/// <param name="fileName">テクスチャのファイルパス</param>
void CubeMapComponent::CreateTexture(const std::string& fileName)
{
	mTexture = new Texture();
	mTexture->LoadCubeMap(fileName);
}

/// <summary>
/// 描画処理
/// </summary>
/// <param name="shader">使用するシェーダー</param>
void CubeMapComponent::Draw(Shader* shader)
{
	if (mVisible)
	{
		glDepthFunc(GL_LEQUAL);
		RENDERER->GetCubeMapVerts()->SetActive();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture->GetTextureID());

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}
}
