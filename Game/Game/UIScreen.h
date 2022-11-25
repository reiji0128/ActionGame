#pragma once
#include "Math.h"
#include <vector>

// �O���錾
class Shader;
class Texture;

class UIScreen
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="offset">���΍��W</param>
	/// <param name="scale">�X�P�[��</param>
	/// <param name="alpha">���ߓx</param>
	UIScreen(const Vector2& offset, const Vector2& scale, const float& alpha);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~UIScreen();

	/// <summary>
	/// �X�V����
	/// </summary>
	virtual void Update(float deltaTime) {};

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() = 0;

protected:
	// ���΍��W
	Vector2 mOffset;

	// �X�P�[��
	Vector2 mScale;

	// ���ߓx
	float mAlpha;

	// �X�N���[���̕�
	int mScreenWidth;

	// �X�N���[���̍���
	int mScreenHeight;

	// Texture�N���X�̃|�C���^
	Texture* mTexture;
};
