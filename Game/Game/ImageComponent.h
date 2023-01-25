#pragma once
#include "UIComponent.h"

class ImageComponent : public UIComponent
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���|�[�l���g�̏��L��</param>
	/// <param name="texture">�e�N�X�`��</param>
	/// <param name="drawOrder">�`�揇��</param>
	ImageComponent(UIScreen* owner,Texture* texture, int drawOrder = 100);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~ImageComponent();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="shader">�g�p����V�F�[�_�[</param>
	void Draw(Shader* shader) override;
};