﻿#include "MeshComponent.h"
#include "Game.h"
#include "Shader.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include <typeinfo>

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="owner">ゲームオブジェクトのポインタ</param>
/// <param name="shaderTag">適用するシェーダーのタグ</param>
/// <param name="isSkeletal">スケルタルモデルかどうか?</param>
MeshComponent::MeshComponent(GameObject* owner, ShaderTag shaderTag, bool isSkeletal)
	:Component(owner)
	,mMesh(nullptr)
	,mTextureIndex(0)
	,mNormalMapIndex(1)
	,mVisible(true)
	,mIsSkeletal(isSkeletal)
	,mShaderTag(shaderTag)
{
	GAMEINSTANCE.GetRenderer()->AddMeshComponent(this,mShaderTag);
	printf("new MeshComponent : [%5d] owner->( 0x%p )\n", GetID(), owner);
}

/// <summary>
/// デストラクタ
/// </summary>
MeshComponent::~MeshComponent()
{
	printf("remove MeshComponent : [%5d] owner->( 0x%p )\n", GetID(), mOwner);
	GAMEINSTANCE.GetRenderer()->RemoveMeshComponent(this,mShaderTag);
}

/// <summary>
/// メッシュコンポーネントの描画処理
/// </summary>
/// <param name="shader">使用するシェーダーのポインタ</param>
void MeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// ワールド変換をセット
		shader->SetMatrixUniform("uWorldTransform",
			mOwner->GetWorldTransform());
		// スペキュラ強度セット
		shader->SetFloatUniform("uSpecPower", 100);
		// アクティブテクスチャセット
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}

		// 法線マップを使うのであればテクスチャをアクティブに
		if (mMesh->GetUseNormalMapFlag())
		{
			Texture* n = mMesh->GetTexture(mNormalMapIndex);
			if (n)
			{
				n->SetActiveNormalMap();
			}
		}

		// 頂点配列をアクティブにセット
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();

		// 描画
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}