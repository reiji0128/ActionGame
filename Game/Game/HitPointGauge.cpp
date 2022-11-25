#include "HitPointGauge.h"
#include "Game.h"
#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "GameObject.h"
#include "GraphicResourceManager.h"
#include "IHitPointGetter.h"

HitPointGauge::HitPointGauge(const Vector2& offset, const Vector2& scale, float alpha, const char* textureFilePass, IHitPointGetter* playerParameterGetter)
	:UIScreen(offset, scale, alpha)
	,mNowHP(0)
{
	// テクスチャの生成
	mTexture = GraphicResourceManager::LoadTexture(textureFilePass);

	// インターフェイスから最大体力を取得
	mHitPointGetter = playerParameterGetter;
	mMaxHP = mHitPointGetter->GetMaxHP();
}

HitPointGauge::~HitPointGauge()
{
}

void HitPointGauge::Draw()
{
	// 頂点配列をアクティブに変更
	RENDERER->ChangeActiveHPGaugeVertex();
	
	// 体力ゲージのスケール計算
	CalcGaugeScale();

	Shader* useShader = nullptr;
	useShader = GraphicResourceManager::FindUseShader(ShaderTag::TILEMAP);

	// テクスチャの幅・高さのスケール行列の計算
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexture->GetWidth())  * resultScale,
		                                    static_cast<float>(mTexture->GetHeight()) * mScale.y,
                                            1.0f);

	// スクリーン位置の平行移動
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(mOffset.x - (mScreenWidth * 0.5f),
		                                                 (mScreenHeight * 0.5f) - mOffset.y,
		                                                  0.0f));

	// ワールド変換行列の計算
	Matrix4 world = scaleMat * transMat;

	Vector2 tileSplitNum(static_cast<float>(1), static_cast<float>(1));

	// シェーダーに必要な値を渡す
	useShader->SetActive();
	useShader->SetMatrixUniform("uWorldTransform", world);
	useShader->SetIntUniform("uTileIndex", 0);
	useShader->SetVector2Uniform("uTileSetSplitNum", tileSplitNum);
	useShader->SetFloatUniform("uAlpha", mAlpha);
	useShader->SetMatrixUniform("uViewProj", RENDERER->GetSimpleViewProjMatrix());

	// テクスチャのセット
	mTexture->SetActive();

	// 四角形描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void HitPointGauge::CalcGaugeScale()
{
	// 現在の体力を取得
	mNowHP = mHitPointGetter->GetHP();

	//体力比率計算
	float hpRate = mNowHP / mMaxHP;
	resultScale = hpRate / 2 * mScale.x;
}
