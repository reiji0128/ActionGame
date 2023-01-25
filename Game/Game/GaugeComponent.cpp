#include "GaugeComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "UIScreen.h"

GaugeComponent::GaugeComponent(UIScreen* owner, Texture* texture,float maxValue,int drawOrder)
	:UIComponent(owner,UITag::Gauge,drawOrder)
	,mNowValue(maxValue)
	,mMaxValue(maxValue)
{
	// テクスチャのセット
	mTexture = texture;

	// 座標・スケール・透過度のセット
	mOffset = owner->GetOffset();
	mScale = owner->GetScale();
	mAlpha = owner->GetAlpha();

	// ゲージのスケールの計算
	CalcGaugeScale(mNowValue);

	// レンダークラスに追加
	RENDERER->AddUI(this);
}

GaugeComponent::~GaugeComponent()
{
	// レンダークラスから削除
	RENDERER->RemoveUI(this);
}

void GaugeComponent::Draw(Shader* shader)
{
	// テクスチャの幅・高さのスケール行列の計算
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexture->GetWidth()) * mResultScale,
		                                    static_cast<float>(mTexture->GetHeight()) * mScale.y,
		                                    1.0f);

	// スクリーン位置の平行移動
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(mOffset.x - (mScreenWidth * 0.5f),
		                                          (mScreenHeight * 0.5f) - mOffset.y,
		                                          0.0f));

	// ワールド変換行列の計算
	Matrix4 world = scaleMat * transMat;

	Vector2 tileSplitNum(static_cast<float>(1), static_cast<float>(1));

	// シェーダーに必要な値をセット
	shader->SetActive();
	shader->SetMatrixUniform("uWorldTransform", world);
	shader->SetIntUniform("uTileIndex", 0);
	shader->SetVector2Uniform("uTileSetSplitNum", tileSplitNum);
	shader->SetFloatUniform("uAlpha", mAlpha);
	shader->SetMatrixUniform("uViewProj", RENDERER->GetSimpleViewProjMatrix());

	// テクスチャをアクティブにセット
	mTexture->SetActive();

	// 四角形描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void GaugeComponent::CalcGaugeScale(const float& nowValue)
{
	mNowValue = nowValue;

	//体力の比率計算
	float hpRate = mNowValue / mMaxValue;
	mResultScale = hpRate / 2 * mScale.x;
}
