#include "EnemyStateIdle.h"
#include "EnemyObject.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"

EnemyStateIdle::EnemyStateIdle()
{
}

EnemyStateIdle::~EnemyStateIdle()
{
}


EnemyState EnemyStateIdle::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	// �v���C���[�̍��W�擾
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// �G�̍��W�擾
	Vector3 mPosition = owner->GetPosition();
	// �G����v���C���[�Ɍ������x�N�g�����Z�o
	Vector3 enemyToPlayerVec = playerPos - mPosition;
	float length = enemyToPlayerVec.Length();

	// �G�̌����̎擾
	Vector3 dir = owner->GetDirection();

	//return EnemyState::STATE_IDLE;

	//�ߋ����U���̋����܂ŋ߂Â��Ă���,�X�e�[�g�̃^�C�}�[��0�ɂȂ�����ߋ����U����ԂɈڍs
	if (InViewAngle(dir,enemyToPlayerVec,length) || mStateTimer < 0.0f)
	{
		// �������Z�o
		int probability = rand() % 100;

		// 40%�̊m���łЂ������U���ڍs
		if (0 <= probability && probability < 40)
		{
			return EnemyState::STATE_PRELIMINARY_ACTION_HAND;
		}

		// 40%�̊m���Ŋ��݂��U���Ɉڍs
		if (40 <= probability && probability < 80)
		{
			return EnemyState::STATE_PRELIMINARY_ACTION_BITE;
		}

		// 10%�̊m���ŉ΃u���X�U���Ɉڍs
		if (80 <= probability && probability < 90)
		{
			return EnemyState::STATE_FIRE_BREATH;
		}

		// 10%�̊m���ŉ΋��U���Ɉڍs
		if (90 <= probability && probability < 100)
		{
			return EnemyState::STATE_FIRE_BALL;
		}
	}

	// �v���C���[�Ƃ̋���������������
	if (length <= mFarLength && mStateTimer < 0.0f)
	{
		// �������Z�o
		int probability = rand() % 100;
		
		// 50%�̊m���Ń�����Ԉڍs
		if (0 <= probability && probability < 30)
		{
			return EnemyState::STATE_WALK;
		}

		// 50%�̊m���Ń�����Ԉڍs
		if (30 <= probability && probability < 40)
		{
			return EnemyState::STATE_RUN;
		}

		// 25%�̊m���Ńu���X�U���Ɉڍs
		if (40 <= probability && probability < 70)
		{
			return EnemyState::STATE_FIRE_BREATH;
		}

		// 25%�̊m���ŉ΋��U���Ɉڍs
		if (70 <= probability && probability < 100)
		{
			return EnemyState::STATE_FIRE_BALL;
		}
	}
	
	// ����ɓ����Ă��Ȃ�������
	if (!InViewAngle(dir, enemyToPlayerVec, length))
	{
		enemyToPlayerVec.z = 0.0f;
		enemyToPlayerVec.Normalize();
		owner->SetDirection(enemyToPlayerVec);
	}

	if (mStateTimer < 0.0f)
	{
		return EnemyState::STATE_RUN;
	}

	if (owner->GetIsDamage())
	{
		//return EnemyState::STATE_DAMAGE;
	}

	enemyToPlayerVec.z = 0.0f;
	enemyToPlayerVec.Normalize();
	owner->SetDirection(enemyToPlayerVec);

	return EnemyState::STATE_IDLE;
}

void EnemyStateIdle::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_IDLE),0.3f);
	
	Vector3 velosity = Vector3(0, 0, 0);
	owner->SetVelosity(velosity);
	mStateTimer = 1.5f;
}
