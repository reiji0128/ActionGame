#pragma once
#include "HUD.h"

// �O���錾
class VertexArray;

class PlayerHitPointBar : public HUD
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="offset">�I�t�Z�b�g</param>
	/// <param name="scale">�g�嗦</param>
	/// <param name="alpha">���ߓx</param>
	PlayerHitPointBar(const Vector2& offset, const Vector2& scale, float alpha);

	~PlayerHitPointBar();

	void Draw() override;

private:
	VertexArray* mHitPointBarVerts;
};