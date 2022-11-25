#include "PlayerStateFirstAttack.h"
#include "Input.h"
#include "AttackObj.h"

PlayerStateFirstAttack::PlayerStateFirstAttack()
	:NextComboTimer(0.0f)
{
}

PlayerStateFirstAttack::~PlayerStateFirstAttack()
{
}

PlayerState PlayerStateFirstAttack::Update(PlayerObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	NextComboTimer += deltaTime;

	// �U������
	Attack(owner, deltaTime);

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

	// �R���{���͂��������玟�̃R���{�Ɉڍs
	if (IsNextCombo)
	{
		IsNextCombo = false;
		return PlayerState::STATE_SECOND_ATTACK;
	}

	// �_���[�W���󂯂Ă�����_���[�W�󂯂��ԂɈڍs
	if (owner->GetIsDamage())
	{
		return PlayerState::STATE_FLYING_BACK;
	}

	return PlayerState::STATE_FIRST_ATTACK;
}

void PlayerStateFirstAttack::Enter(PlayerObject* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_FIRST_ATTACK),0.5f);
	
	mStateTimer = 1.0f;
	NextComboTimer = 0.0f;

	owner->SetVelocity(Vector3(0, 0, 0));
}

void PlayerStateFirstAttack::Attack(PlayerObject* owner, float deltaTime)
{
	//�U���\���ԂɂȂ�����U��
	if (mStateTimer < 0.0f)
	{
		new AttackObj(Tag::PlayerFirstAttack, owner->GetPosition(), owner->GetDirection(), 300.0f, 0.01f);

		// �^�C�}�[�𖳌���Ƀ��Z�b�g���čU�����J��Ԃ��Ȃ��悤�ɂ���
		mStateTimer = FLT_MAX;
	}
}

void PlayerStateFirstAttack::InputProcess()
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

	// ���R���{�Ɉڍs�ł���t���[���ȍ~�ɍU�����͂����ꂽ��
	if (INPUT_INSTANCE.IsKeyPushdown(BUTTON_B) && NextComboTimer >= NextComboLimitTimer)
	{
		IsNextCombo = true;
	}

}
