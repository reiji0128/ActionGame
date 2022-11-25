#include "PlayerStateSecondAttack.h"
#include "Input.h"
#include "AttackObj.h"

PlayerStateSecondAttack::PlayerStateSecondAttack()
{
}

PlayerStateSecondAttack::~PlayerStateSecondAttack()
{
}

PlayerState PlayerStateSecondAttack::Update(PlayerObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;
	// �U������
	Attack(owner,deltaTime);

	// ���͏���
	InputProcess();

	// �A�j���[�V�����̍Đ����I����Ă��邩
	if (!mSkelComp->IsPlaying())
	{
		// �ړ����͂��Ȃ�������A�C�h����ԂɈڍs
		// �ړ����͂����ꂽ�烉����ԂɈڍs
		if (IsIdle)
		{
			return PlayerState::STATE_IDLE;
		}
		else
		{
			return PlayerState::STATE_RUN;
		}
	}

	// �_���[�W���󂯂Ă�����_���[�W�󂯂��ԂɈڍs
	if (owner->GetIsDamage())
	{
		return PlayerState::STATE_FLYING_BACK;
	}

	return PlayerState::STATE_SECOND_ATTACK;
}

void PlayerStateSecondAttack::Enter(PlayerObject* owner, float deltaTime)
{
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_SECOND_ATTACK),0.7f);
	mStateTimer = 1.0f;

	owner->SetVelocity(Vector3(0, 0, 0));
}

void PlayerStateSecondAttack::Attack(PlayerObject* owner, float deltaTime)
{
	//�U���\���ԂɂȂ�����U��
	if (mStateTimer < 0.0f)
	{
		new AttackObj(Tag::PlayerSecondAttack, owner->GetPosition(), owner->GetDirection(), 300.0f, 0.01f);

		// �^�C�}�[�𖳌���Ƀ��Z�b�g���čU�����J��Ԃ��Ȃ��悤�ɂ���
		mStateTimer = FLT_MAX;
	}
}

void PlayerStateSecondAttack::InputProcess()
{
	// �R���g���[���̍��X�e�B�b�N�����͂��ꂽ��
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	IsControllerInputOff = !INPUT_INSTANCE.IsLStickMove();

	// �ړ����͂�����Ă��Ȃ���
	IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_W) &
		     INPUT_INSTANCE.IsKeyOff(KEY_S) &
		     INPUT_INSTANCE.IsKeyOff(KEY_D) &
		     INPUT_INSTANCE.IsKeyOff(KEY_A) &
		     IsControllerInputOff;
}
