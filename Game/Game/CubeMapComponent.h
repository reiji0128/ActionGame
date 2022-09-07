#pragma once
#include "Component.h"
#include <string>

class GameObject;
class Shader;
class Texture;

class CubeMapComponent : public Component
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�Q�[���I�u�W�F�N�g�̃|�C���^</param>
	CubeMapComponent(GameObject* owner);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~CubeMapComponent();

	/// <summary>
	/// �e�N�X�`���̐���
	/// </summary>
	/// <param name="fileName"></param>
	void CreateTexture(const std::string& fileName);

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="shader">�g�p����V�F�[�_�[</param>
	void Draw(Shader* shader);

// �Z�b�^�[�@//

	/// <summary>
	/// �\���t���O�̃Z�b�g
	/// </summary>
	/// <param name="visible">
	/// true  : �\������
	/// false : �\�����Ȃ�
	/// </param>
	void SetVisible(bool visible) { mVisible = visible;}

// �Q�b�^�[ //

	/// <summary>
	/// �\���t���O�̎擾
	/// </summary>
	/// <returns>
	/// true  : �\������
	/// false : �\�����Ȃ�
	/// </returns>
	bool GetVisible() const { return mVisible; }

private:
	// �\���t���O
	bool mVisible;

	// �e�N�X�`���̃|�C���^
	Texture* mTexture;
};