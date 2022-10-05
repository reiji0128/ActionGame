#include "PlayerStateFirstAttack.h"
#include "Input.h"
#include "AttackObj.h"

PlayerStateFirstAttack::PlayerStateFirstAttack()
{
}

PlayerStateFirstAttack::~PlayerStateFirstAttack()
{
}

PlayerState PlayerStateFirstAttack::Update(PlayerObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	// �R���g���[���̍��X�e�B�b�N�����͂��ꂽ��
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
	
	Attack(owner, deltaTime);
	
	IsFirstAttack = true;
}

void PlayerStateFirstAttack::Attack(PlayerObject* owner, float deltaTime)
{
	//�U���\���ԂɂȂ�����U��
	if (mStateTimer < 0.0f)
	{
		new AttackObj(Tag::EnemyAttackHand, owner->GetPosition(), owner->GetDirection(), 150.0f, 0.3f);

		// �^�C�}�[�𖳌���Ƀ��Z�b�g���čU�����J��Ԃ��Ȃ��悤�ɂ���
		mStateTimer = FLT_MAX;
	}
}
