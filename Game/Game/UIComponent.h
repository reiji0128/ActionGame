#pragma once
#include "Math.h"
#include "UITag.h"

// �O���錾
class UIScreen;
class Shader;
class Texture;

class UIComponent
{
public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="owner">�R���|�[�l���g�̏��L��</param>
	/// <param name="drawingSequence">�`�揇��</param>
	UIComponent(UIScreen* owner, UITag tag, int drawOrder = 100);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~UIComponent();

	/// <summary>
	/// �`�揈��
	/// </summary>
	/// <param name="shader"></param>
	virtual void Draw(Shader* shader) = 0;

// �Q�b�^�[ //

	/// <summary>
	/// �`�揇�Ԃ̎擾
	/// </summary>
	/// <returns>�`�揇��</returns>
	const int GetDrawOrder() const { return mDrawOrder; }

	/// <summary>
	/// �\���t���O�̎擾
	/// </summary>
	/// <returns>
	/// true  : �\������
	/// false : �\�����Ȃ�
	/// </returns>
	const bool GetVisible() const { return mVisible; }

	/// <summary>
	/// UI�^�O�̎擾
	/// </summary>
	/// <returns>UI1�̃^�O</returns>
	const UITag GetUITag() const { return mUITag; }

protected:

	// �X�N���[���̕�
	int mScreenWidth;

	// �X�N���[���̍���
	int mScreenHeight;

	// ���΍��W
	Vector2 mOffset;

	// �X�P�[��
	Vector2 mScale;

	// ���ߓx
	float mAlpha;

	// �e�N�X�`��
	Texture* mTexture;

	// UI�̃^�O
	UITag mUITag;

	// �`�揇��
	int mDrawOrder;

	// �\�����邩
	bool mVisible;

	// �R���|�[�l���g�̏��L��
	UIScreen* mOwner;
};