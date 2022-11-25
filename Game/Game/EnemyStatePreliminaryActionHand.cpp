#include "EnemyStatePreliminaryActionHand.h"
#include "EnemyObject.h"
#include "Game.h"
#include "AudioManager.h"
#include "SkeletalMeshComponent.h"

EnemyStatePreliminaryActionHand::EnemyStatePreliminaryActionHand()
{
}

EnemyStatePreliminaryActionHand::~EnemyStatePreliminaryActionHand()
{
}

EnemyState EnemyStatePreliminaryActionHand::Update(EnemyObject* owner, float deltaTime)
{
	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}

	// �A�j���[�V�����̍Đ��͏I����Ă��邩
	if (!mSkelComp->IsPlaying())
	{
		return EnemyState::STATE_ATTACK_HAND;
	}

	return EnemyState::STATE_PRELIMINARY_ACTION_HAND;
}

void EnemyStatePreliminaryActionHand::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�j���[�V�����̍Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_PRELIMINARY_ACTION_HAND),0.2f);

	Vector3 velosity = Vector3(0, 0, 0);
	owner->SetVelosity(velosity);

	AUDIO->PlaySoundW("Assets/Sound/PreliminaryAction01.wav");
}
