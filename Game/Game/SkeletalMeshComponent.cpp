﻿#include "SkeletalMeshComponent.h"
#include "Shader.h"
#include "Mesh.h"
#include "GameObject.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Animation.h"
#include "Skeleton.h"
#include <string>

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="owner">コンポーネントを所有するゲームオブジェクトのポインタ</param>
/// <param name="shaderTag">適用するシェーダーのタグ</param>
SkeletalMeshComponent::SkeletalMeshComponent(GameObject* owner, ShaderTag shaderTag)
	:MeshComponent(owner,shaderTag,true)
	, mSkeleton(nullptr)
{
}

/// <summary>
/// スケルタルメッシュコンポーネントの描画処理
/// </summary>
/// <param name="shader"Shaderのポインタ>使用するシェーダー</param>
void SkeletalMeshComponent::Draw(Shader* shader)
{
	if (mMesh)
	{
		// ワールド行列のセット
		shader->SetMatrixUniform("uWorldTransform",mOwner->GetWorldTransform());
		// 行列パレットをセット    
		shader->SetMatrixUniforms("uMatrixPalette", &mPalette.mEntry[0],MAX_SKELETON_BONES);
		// スペキュラー強度をセット
		shader->SetFloatUniform("uSpecPower", 100);
		// 輝度のセット
		float luminance = mMesh->GetLuminance();
		shader->SetFloatUniform("uLuminance", luminance);

		// テクスチャをセット 
		Texture* t = mMesh->GetTexture(mTextureIndex);
		if (t)
		{
			t->SetActive();
		}

		// メッシュの頂点配列をアクティブにセット
		VertexArray* va = mMesh->GetVertexArray();
		va->SetActive();
		
		// 頂点の描画
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

/// <summary>
/// スケルタルメッシュコンポーネントの更新処理
/// </summary>
/// <param name="deltaTime">1フレームの経過時間</param>
void SkeletalMeshComponent::Update(float deltaTime)
{
	if (mAnimation && mSkeleton)
	{
		mAnimTime += deltaTime * mAnimPlayRate;

		// アニメーションがループアニメーションなら巻き戻し処理
		if (mAnimation->IsLoopAnimation())
		{
			// Wrap around anim time if past duration                         アニメを巻き戻して再生
			while (mAnimTime > mAnimation->GetDuration())
			{
				mAnimTime -= mAnimation->GetDuration();
			}
		}
		// ループしないアニメで再生時間超えたら最終時間までとする
		else if(mAnimTime > mAnimation->GetDuration())
		{
			mAnimTime = mAnimation->GetDuration();
		}
		// Recompute matrix palette                                      行列パレットの再計算
		ComputeMatrixPalette();
	}
}

/// <summary>
/// アニメーションの再生
/// </summary>
/// <param name="anim">Animationのポインタ</param>
/// <param name="playRate">再生割合</param>
/// <returns>アニメーションの残り長さを返す</returns>
float SkeletalMeshComponent::PlayAnimation(const Animation* anim, float playRate)  // アニメーションの再生
{
	mAnimation = anim;
	mAnimTime = 0.0f;
	mAnimPlayRate = playRate;

	if (!mAnimation) { return 0.0f; }

	ComputeMatrixPalette();

	return mAnimation->GetDuration();
}

/// <summary>
/// 現在再生中のアニメーションは再生中か？
/// </summary>
/// <returns>
/// true  : 再生中
/// false : 再生終了
/// </returns>
bool SkeletalMeshComponent::IsPlaying()
{
	if (!mAnimation->IsLoopAnimation())
	{
		if (mAnimTime >= mAnimation->GetDuration())
		{
			return false;
		}
	}
	return true;
}

/// <summary>
/// GetBoneIndexFromName関数の呼び出し
/// </summary>
/// <param name="BoneName">取得したいボーン名</param>
/// <returns>ボーンのインデックスを取得</returns>
int SkeletalMeshComponent::CallGetBoneIndexFromName(std::string BoneName)
{
	//ボーンの名前からボーンのインデックスを取得
	return mSkeleton->GetBoneIndexFromName(BoneName);
}

/// <summary>
/// インデックスからボーンの行列を取得
/// </summary>
/// <param name="BoneIndex">取得したいボーンのインデックス</param>
/// <returns>ボーンの行列</returns>
const Matrix4 SkeletalMeshComponent::GetBoneMatrix(int BoneIndex) const
{
	return mPalette.mEntry[BoneIndex] * mOwner->GetWorldTransform();
}

/// <summary>
/// 行列パレットの計算
/// </summary>
void SkeletalMeshComponent::ComputeMatrixPalette()
{
	// グローバル逆バインド行列配列の取得
	const std::vector<Matrix4>& globalInvBindPoses = mSkeleton->GetGlobalInvBindPoses();

	// 現在のポーズ行列
	std::vector<Matrix4> currentPoses;
	// アニメ時刻時点のグローバルポーズの取得
	mAnimation->GetGlobalPoseAtTime(currentPoses, mSkeleton, mAnimTime);

	// それぞれのボーンの行列パレットのセット
	for (size_t i = 0; i < mSkeleton->GetNumBones(); i++)
	{
		// 行列パレット[i] = グローバル逆バインド行列[i]　×　現在のポーズ行列[i]  (iはボーンID)         
		mPalette.mEntry[i] = globalInvBindPoses[i] * currentPoses[i];
	}
}
