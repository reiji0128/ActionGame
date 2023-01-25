#include "ImageComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "Texture.h"
#include "Shader.h"
#include "UIScreen.h"

ImageComponent::ImageComponent(UIScreen* owner, Texture* texture, int drawOrder)
	:UIComponent(owner,UITag::Image,drawOrder)
{
	// テクスチャのセット
	mTexture = texture;

	// 座標・スケール・透過度のセット
	mOffset = owner->GetOffset();
	mScale = owner->GetScale();
	mAlpha = owner->GetAlpha();

	// レンダークラスに追加
	RENDERER->AddUI(this);
}

ImageComponent::~ImageComponent()
{
	// レンダークラスから削除
	RENDERER->RemoveUI(this);
}

void ImageComponent::Draw(Shader* shader)
{
	// テクスチャの幅・高さのスケール行列の計算
	Matrix4 scaleMat = Matrix4::CreateScale(static_cast<float>(mTexture->GetWidth()) * mScale.x,
		                                    static_cast<float>(mTexture->GetHeight()) * mScale.y,
		                                    1.0f);

	// スクリーン位置の平行移動
	Matrix4 transMat = Matrix4::CreateTranslation(Vector3(mOffset.x - (mScreenWidth * 0.5f),
		                                                 (mScreenHeight * 0.5f) - mOffset.y,
		                                                 0.0f));

	// ワールド変換行列の計算
	Matrix4 world = scaleMat * transMat;

	// シェーダーに必要な値をセット
	shader->SetActive();
	shader->SetMatrixUniform("uWorldTransform", world);
	shader->SetMatrixUniform("uViewProj", RENDERER->GetSimpleViewProjMatrix());
	shader->SetFloatUniform("uAlpha", mAlpha);

	// テクスチャをアクティブにセット
	mTexture->SetActive();

	// 描画
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}
