#include "PlayerStateDeath.h"


PlayerStateDeath::PlayerStateDeath()
{
}

PlayerStateDeath::~PlayerStateDeath()
{
}

PlayerState PlayerStateDeath::Update(PlayerObject* owner, float deltaTime)
{
	//�@�A�j���[�V�����̍Đ����I�������
	if (!mSkelComp->IsPlaying())
	{
		// �v���C���[��Dead��Ԃ�
		owner->SetState(owner->EDead);
	}

	return PlayerState::STATE_DEATH;
}

void PlayerStateDeath::Enter(PlayerObject* owner, float deltaTime)
{
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_DEATH));
}
