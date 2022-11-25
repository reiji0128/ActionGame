#include "EnemyStateDamage.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"

EnemyStateDamage::EnemyStateDamage()
{
}

EnemyStateDamage::~EnemyStateDamage()
{
}

EnemyState EnemyStateDamage::Update(EnemyObject* owner, float deltaTime)
{
	
	//// �������Z�o
	//int probability = rand() % 100;

	//// 40%�̊m���łЂ������U���ڍs
	//if (0 <= probability && probability < 40)
	//{
	//	return EnemyState::STATE_PRELIMINARY_ACTION_HAND;
	//}

	//// 40%�̊m���Ŋ��݂��U���Ɉڍs
	//if (40 <= probability && probability < 80)
	//{
	//	return EnemyState::STATE_PRELIMINARY_ACTION_BITE;
	//}

	//// 10%�̊m���ŉ΃u���X�U���Ɉڍs
	//if (80 <= probability && probability < 90)
	//{
	//	return EnemyState::STATE_WALK;
	//}

	//// 10%�̊m���ŉ΋��U���Ɉڍs
	//if (90 <= probability && probability < 100)
	//{
	//	return EnemyState::STATE_RUN;
	//}
	return EnemyState::STATE_IDLE;
	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}
}

void EnemyStateDamage::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	//mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_DAMAGE),0.6);

	mStateTimer = 2.0f;

	// �_���[�W����
	int damage = owner->GetReceiveDamage();
	owner->Damage(damage);
}
