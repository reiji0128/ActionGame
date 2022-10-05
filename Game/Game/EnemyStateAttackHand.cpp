#include "EnemyStateAttackHand.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"
#include "AttackObj.h"

EnemyStateAttackHand::EnemyStateAttackHand()
{
}

EnemyStateAttackHand::~EnemyStateAttackHand()
{
}

EnemyState EnemyStateAttackHand::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	// �G����v���C���[�Ɍ������x�N�g�����Z�o
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();


	// �A�j���[�V�����͍Đ�����
	if (!mSkelComp->IsPlaying())
	{
		// �v���C���[�Ƃ̋������ǐՋ������Z����
		if (length < retrackingRange)
		{
			return EnemyState::STATE_IDLE;
		}
		else
		{
			return EnemyState::STATE_RUN;
		}
	}

	return EnemyState::STATE_ATTACK_HAND;
}

void EnemyStateAttackHand::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_ATTACK_HAND),0.8f);

	Vector3 velosity = Vector3(0,0,0);
	owner->SetVelosity(velosity);
}

void EnemyStateAttackHand::Attack(EnemyObject* owner, float deltaTime)
{
	//�U���\���ԂɂȂ�����U��
	if (mStateTimer < 0.0f)
	{
		new AttackObj(Tag::EnemyAttackHand, owner->GetPosition(), owner->GetDirection(), 150.0f, 0.3f);
		mStateTimer = FLT_MAX;		// �^�C�}�[�𖳌���Ƀ��Z�b�g���čU�����J��Ԃ��Ȃ��悤�ɂ���
	}
}
