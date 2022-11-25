#include "EnemyStatePreliminaryActionBite.h"
#include "Game.h"
#include "AudioManager.h"
#include "EnemyObject.h"
#include "SkeletalMeshComponent.h"

EnemyStatePreliminaryActionBite::EnemyStatePreliminaryActionBite()
{
}

EnemyStatePreliminaryActionBite::~EnemyStatePreliminaryActionBite()
{
}

EnemyState EnemyStatePreliminaryActionBite::Update(EnemyObject* owner, float deltaTime)
{
	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}

	// �A�j���[�V�����̍Đ��͏I����Ă��邩
	if (!mSkelComp->IsPlaying())
	{
		return EnemyState::STATE_ATTACK_BITE;
	}

	return EnemyState::STATE_PRELIMINARY_ACTION_BITE;
}

void EnemyStatePreliminaryActionBite::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�j���[�V�����̍Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_PRELIMINARY_ACTION_BITE),0.2f);

	Vector3 velosity = Vector3(0, 0, 0);
	owner->SetVelosity(velosity);

	AUDIO->PlaySoundW("Assets/Sound/PreliminaryAction02.wav");
}
