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

	// �V�F�[�_�[�Ƀ��C�g�̍��W�E�f�B�t���[�Y�E�A���r�G���g���Z�b�g
	shader->SetVectorUniform("uLight.position", lightPos);
	shader->SetVectorUniform("uLight.diffuse" , mColor);
	shader->SetVectorUniform("uLight.ambient" , ambient);

	// �X�P�[���E���W�̍s����v�Z
	Matrix4 mat = Matrix4::CreateScale(mRadius);
	mat = mat * Matrix4::CreateTranslation(lightPos);

	// �V�F�[�_�[�Ƀ��f���̍s����Z�b�g
	shader->SetMatrixUniform("model", mat);

	// ���[���h�ϊ����Z�b�g
	shader->SetMatrixUniform("uWorldTransform", mOwner->GetWorldTransform());
	// �X�y�L�������x�Z�b�g
	shader->SetFloatUniform("uSpecPower", 100);

	// ���_�z����A�N�e�B�u�ɃZ�b�g
	VertexArray* va = sphereMesh->GetVertexArray();
	va->SetActive();

	// �`��
	glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
}

void PointLightComponent::CalcAttenuationLightRadius(const float constant, const float linear, const float quadratic)
{
	// ���̌�����茸�����a���v�Z
	float max = std::fmax(std::fmax(mColor.x, mColor.y), mColor.z);
	float radius = (-linear + std::sqrtf((linear * linear) - 4.0f * quadratic *
		           (constant - (256.0f / 4.0f) * max))) / (2.0f * quadratic);

	mRadius = radius;
}
