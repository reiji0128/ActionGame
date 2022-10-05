#include "PointLightComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "Mesh.h"
#include "VertexArray.h"

PointLightComponent::PointLightComponent(GameObject* owner, const Vector3& offset, const Vector3& color)
	:Component(owner)
	,mOffset(offset)
	,mColor(color)
{
	GAMEINSTANCE.GetRenderer()->AddPointLight(this);
	printf("new PointLightComponent : [%5d] owner->( 0x%p )\n", GetID(), owner);
}

PointLightComponent::~PointLightComponent()
{
	printf("remove PointLightComponent : [%5d] owner->( 0x%p )\n", GetID(), mOwner);
	GAMEINSTANCE.GetRenderer()->RemovePointLight(this);
}

void PointLightComponent::Draw(Shader* shader, Mesh* sphereMesh)
{
	Vector3 lightPos = mOwner->GetPosition() + mOffset;
	Vector3 ambient = mColor * 0.2f;

	// シェーダーにライトの座標・ディフューズ・アンビエントをセット
	shader->SetVectorUniform("uLight.position", lightPos);
	shader->SetVectorUniform("uLight.diffuse" , mColor);
	shader->SetVectorUniform("uLight.ambient" , ambient);

	// スケール・座標の行列を計算
	Matrix4 mat = Matrix4::CreateScale(mRadius);
	mat = mat * Matrix4::CreateTranslation(lightPos);

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

void PointLightComponent::CalcAttenuationLightRadius(const float constant, const float linear, const float quadratic)
{
	// 解の公式より減衰半径を計算
	float max = std::fmax(std::fmax(mColor.x, mColor.y), mColor.z);
	float radius = (-linear + std::sqrtf((linear * linear) - 4.0f * quadratic *
		           (constant - (256.0f / 4.0f) * max))) / (2.0f * quadratic);

	mRadius = radius;
}
