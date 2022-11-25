#pragma once
#include "Component.h"
#include "Math.h"

// �O���錾
class GameObject;
class Shader;
class Mesh;


class PointLightComponent : public Component
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���|�[�l���g�����L����Q�[���I�u�W�F�N�g</param>
	/// <param name="offset">���΍��W</param>
	/// <param name="color">���C�g�̐F</param>
	/// <param name="luminance">�P�x</param>
	PointLightComponent(GameObject* owner,const Vector3& offset, const Vector3& color,const float& luminance);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~PointLightComponent();

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void Update(float deltaTime) override;

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="shader">�g�p����V�F�[�_�[</param>
	/// <param name="sphereMesh">�`��Ɏg�����̂̃��b�V��</param>
	void Draw(Shader* shader, Mesh* sphereMesh);

	/// <summary>
	/// ���C�g�̌������a�̌v�Z
	/// </summary>
	/// <param name="constant">�萔</param>
	/// <param name="linear">���`</param>
	/// <param name="quadratic">2�捀</param>
	void CalcAttenuationLightRadius(const float constant, const float linear, const float quadratic);

// �Z�b�^�[ //

	/// <summary>
	/// �\���t���O�̃Z�b�g
	/// </summary>
	/// <param name="visibleFrag"></param>
	void SetIsVisible(bool visibleFrag) { mVisible = visibleFrag; }

	/// <summary>
	/// �F�̃Z�b�g
	/// </summary>
	/// <param name="color">�F</param>
	void SetColor(Vector3 color) { mColor = color; }

private:

	// ���C�g�̍��W
	Vector3 mLightPos;

	// �I�t�Z�b�g
	Vector3 mOffset;

	// �|�C���g���C�g�̐F
	Vector3 mColor;

	// �|�C���g���C�g�̔��a
	float mRadius;

	// �P�x
	float mLuminance;

	// �\���t���O
	bool mVisible;
};
