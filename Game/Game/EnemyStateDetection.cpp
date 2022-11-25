#include "EnemyStateDetection.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"

EnemyStateDetection::EnemyStateDetection()
	:mRunSpeed(200.0f)
{
}

EnemyStateDetection::~EnemyStateDetection()
{
}

EnemyState EnemyStateDetection::Update(EnemyObject* owner, float deltaTime)
{
	// �v���C���[�̍��W�擾
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// �G�̍��W�擾
	Vector3 position = owner->GetPosition();
	// �G����v���C���[�Ɍ������x�N�g�����Z�o
	Vector3 enemyToPlayerVec = playerPos - position;
	float length = enemyToPlayerVec.Length();

	// �G�̌����̎擾
	Vector3 dir = owner->GetDirection();

	if (length <= mNearLength)
	{
		return EnemyState::STATE_PRELIMINARY_ACTION_HAND;
	}

	enemyToPlayerVec.z = 0.0f;
	enemyToPlayerVec.Normalize();
	owner->SetDirection(enemyToPlayerVec);

	MoveCalc(owner, deltaTime, enemyToPlayerVec);

	return EnemyState::STATE_DETECTION;
}

void EnemyStateDetection::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�C�h����Ԃ̃A�j���[�V�������Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_DETECTION),0.4f);

	Vector3 velocity = Vector3::Zero;
	owner->SetVelosity(velocity);

	//owner->ChangeLightColor(Vector3(0, 0, 0));
}

void EnemyStateDetection::MoveCalc(EnemyObject* owner, float deltaTime,const Vector3& targetDir)
{
	//���������̌������擾
	Vector3 dir = targetDir;
	dir.z = 0.0f;
	dir.Normalize();

	Vector3 velocity = dir * mRunSpeed * deltaTime;
	owner->SetVelosity(velocity);

	owner->SetDirection(dir);
	owner->SetComputeWorldTransform();
}
