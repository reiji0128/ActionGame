#include "PlayerStateDamage.h"

PlayerStateDamage::PlayerStateDamage()
{
}

PlayerStateDamage::~PlayerStateDamage()
{
}

PlayerState PlayerStateDamage::Update(PlayerObject* owner, float deltaTime)
{
	// �A�j���[�V�����̍Đ����I�������A�C�h����ԂɈڍs
	if (!mSkelComp->IsPlaying())
	{
		owner->SetIsDamage(false);
		return PlayerState::STATE_IDLE;
	}

	// HP��0�ȉ��ɂȂ����玀�S��ԂɈڍs
	if (owner->GetHP() <= 0)
	{
		return PlayerState::STATE_DEATH;
	}

	return PlayerState::STATE_DAMAGE;
}

void PlayerStateDamage::Enter(PlayerObject* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeltalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_DAMAGE));

	// �_���[�W����
	int damage = owner->GetReceiveDamage();
	owner->Damage(damage);
}
