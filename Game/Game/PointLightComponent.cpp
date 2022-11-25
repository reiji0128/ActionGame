#include "PointLightComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "Mesh.h"
#include "VertexArray.h"

PointLightComponent::PointLightComponent(GameObject* owner, const Vector3& offset, const Vector3& color,const float& luminance)
	:Component(owner)
	,mLightPos(Vector3::Zero)
	,mOffset(offset)
	,mColor(color)
	,mRadius(0.0f)
	,mVisible(true)
	,mLuminance(luminance)
{
	GAMEINSTANCE.GetRenderer()->AddPointLight(this);
	printf("new PointLightComponent : [%5d] owner->( 0x%p )\n", GetID(), owner);

	mLightPos = mOwner->GetPosition() + mOffset;

	const float constant  = 1.0f;  // 定数
	const float linear    = 0.7f;  // 線形
	const float quadratic = 1.8f;  // 2乗項

	// ライト減衰半径の計算
	CalcAttenuationLightRadius(constant, linear, quadratic);
}

PointLightComponent::~PointLightComponent()
{
	printf("remove PointLightComponent : [%5d] owner->( 0x%p )\n", GetID(), mOwner);
	GAMEINSTANCE.GetRenderer()->RemovePointLight(this);
}

void PointLightComponent::Update(float deltaTime)
{
	mLightPos = mOwner->GetPosition() + mOffset;
}

void PointLightComponent::Draw(Shader* shader, Mesh* sphereMesh)
{
	if (mVisible)
	{
		Vector3 ambient = mColor * 0.2f;

		// シェーダーにライトの座標・ディフューズ・アンビエントをセット
		shader->SetVectorUniform("uLight.position", mLightPos);
		shader->SetVectorUniform("uLight.diffuse", mColor);
		shader->SetVectorUniform("uLight.ambient", ambient);
		shader->SetFloatUniform("luminance", 10.0f);

		// スケール・座標の行列を計算
		Matrix4 mat = Matrix4::CreateScale(mRadius);
		mat = mat * Matrix4::CreateTranslation(mLightPos);

		// シェーダーにモデルの行列をセット
		shader->SetMatrixUniform("model", mat);

		// ワールド変換をセット
		shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
		// スペキュラ強度セット
		shader->SetFloatUniform("uSpecPower", 100);

		// 頂点配列をアクティブにセット
		VertexArray* va = sphereMesh->GetVertexArray();
		va->SetActive();

		// 描画
		glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
	}
}

void PointLightComponent::CalcAttenuationLightRadius(const float constant, const float linear, const float quadratic)
{
	// 解の公式より減衰半径を計算
	float max = std::fmax(std::fmax(mColor.x, mColor.y), mColor.z);

	float radius = (-linear + std::sqrtf(linear * linear - 4.0f * quadratic *
		           (constant - (256.0f / 5.0f) * max))) / (2.0f * quadratic);

	mRadius = radius;
}
