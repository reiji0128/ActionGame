#include "EnemyStaeBase.h"
#include "Game.h"

EnemyStateBase::EnemyStateBase()
	:mStateTimer(0.0f)
	,mSkelComp(nullptr)
{
}

void EnemyStateBase::RotateProcess(EnemyObject* owner, float deltaTime)
{
	// ���݂̌���
	Vector3 nowDirection = owner->GetDirection();

	// ���݂̍��W
	Vector3 ownerPos = owner->GetPosition();

	// �v���C���[�̍��W
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();

	// ��]����
	if (mRotateNow)
	{
		// ��̃x�N�g�����قړ�����
		if (isNearAngle(nowDirection, mAimVec))
		{
			mRotateNow = false;

			// �����̃Z�b�g
			owner->SetDirection(nowDirection);
		}

		// ��]����
		nowDirection = zRotateToAimVec(nowDirection, mAimVec, mRotateSpeed * deltaTime);
		owner->SetDirection(nowDirection);
		owner->RotateToNewForward();
	}
}

bool EnemyStateBase::InViewAngle(const Vector3& from, const Vector3& to, float length)
{
	// ��̃x�N�g������p�x(�ʓx�@)���v�Z
	float angle = Vector3::Angle(from, to);

	// ����͈͓̔��ɓ����Ă�����
	if (angle < viewAngle && length < mNearLength)
	{
		return true;
	}

	return false;
}
