#include "PlayerStateRollForward.h"
#include "Input.h"

PlayerStateRollForward::PlayerStateRollForward()
	:mSpeed(300)
{
}

PlayerStateRollForward::~PlayerStateRollForward()
{
}

PlayerState PlayerStateRollForward::Update(PlayerObject* owner, float deltaTime)
{
	// ��]�̌v�Z
	RollCalc(owner, deltaTime);

	// �A�j���[�V�����̍Đ����I����Ă�����
	if (!mSkelComp->IsPlaying())
	{
		owner->SetVelocity(Vector3::Zero);

		return PlayerState::STATE_IDLE;
	}

	// �_���[�W���󂯂Ă�����_���[�W�󂯂��ԂɈڍs
	if (owner->GetIsDamage())
	{
		return PlayerState::STATE_FLYING_BACK;
	}

	return PlayerState::STATE_ROLL_FORWARD;
}

void PlayerStateRollForward::Enter(PlayerObject* owner, float deltaTime)
{
	// �A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_ROLL_FORWARD), 0.8f);

	// ���W�̎擾
	mOwnerPos = owner->GetPosition();

	// �O���x�N�g���̎擾
	mCharaForwardVec = owner->GetDirection();
}

void PlayerStateRollForward::RollCalc(PlayerObject* owner, float deltaTime)
{
	Vector3 upVec(0.0f,0.0f,1.0f);
	Vector3 rightVec;

	// �O���x�N�g���Ə㎲(Z��)����E�x�N�g�����Z�o
	rightVec = Vector3::Cross(mCharaForwardVec, upVec);

	if (mCharaForwardVec.Length() > 0.5)
	{
		mCharaForwardVec.Normalize();;
	}

	// �O�����ɉ�]
	mCharaForwardVec.z = 0;
	Vector3 velocity;
	velocity += mSpeed * mCharaForwardVec;

	// �ō����x�𒴂��Ă����璲��
	Vector3 horizonSpeed = velocity;
	if (horizonSpeed.Length() > mSpeed * deltaTime)
	{
		horizonSpeed = mCharaForwardVec * mSpeed * deltaTime;
		velocity.x = horizonSpeed.x;
		velocity.y = horizonSpeed.y;
		velocity.z = 0;
	}

	// �X�s�[�h�E�ϊ��s��̍Čv�Z�̕K�v���Z�b�g
	owner->SetVelocity(velocity);
	owner->SetComputeWorldTransform();
}
