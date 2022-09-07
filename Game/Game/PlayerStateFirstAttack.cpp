#include "PlayerStateFirstAttack.h"
#include "Input.h"

PlayerStateFirstAttack::PlayerStateFirstAttack()
{
}

PlayerStateFirstAttack::~PlayerStateFirstAttack()
{
}

PlayerState PlayerStateFirstAttack::Update(PlayerObject* owner, float deltaTime)
{
	// �R���g���[�����͂��ꂽ��
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	bool isContollerInputOff = !INPUT_INSTANCE.IsLStickMove();

	bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_W) &
		          INPUT_INSTANCE.IsKeyOff(KEY_S) &
		          INPUT_INSTANCE.IsKeyOff(KEY_D) &
		          INPUT_INSTANCE.IsKeyOff(KEY_A) &
		isContollerInputOff;

	// �ړ����͂��Ȃ�������
	if (IsIdle)
	{
		return PlayerState::STATE_IDLE;
	}
	else
	{
		return PlayerState::STATE_RUN;
	}

}

void PlayerStateFirstAttack::Enter(PlayerObject* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_FIRST_ATTACK));
}

void PlayerStateFirstAttack::FirstAttack(PlayerObject* owner, float deltaTime)
{
}
