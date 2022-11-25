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

	const float constant  = 1.0f;  // �萔
	const float linear    = 0.7f;  // ���`
	const float quadratic = 1.8f;  // 2�捀

	// ���C�g�������a�̌v�Z
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

		// �V�F�[�_�[�Ƀ��C�g�̍��W�E�f�B�t���[�Y�E�A���r�G���g���Z�b�g
		shader->SetVectorUniform("uLight.position", mLightPos);
		shader->SetVectorUniform("uLight.diffuse", mColor);
		shader->SetVectorUniform("uLight.ambient", ambient);
		shader->SetFloatUniform("luminance", 10.0f);

		// �X�P�[���E���W�̍s����v�Z
		Matrix4 mat = Matrix4::CreateScale(mRadius);
		mat = mat * Matrix4::CreateTranslation(mLightPos);

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
}

void PointLightComponent::CalcAttenuationLightRadius(const float constant, const float linear, const float quadratic)
{
	// ���̌�����茸�����a���v�Z
	float max = std::fmax(std::fmax(mColor.x, mColor.y), mColor.z);

	float radius = (-linear + std::sqrtf(linear * linear - 4.0f * quadratic *
		           (constant - (256.0f / 5.0f) * max))) / (2.0f * quadratic);

	mRadius = radius;
}
