#pragma once
#include "EnemyObject.h"

class EnemyObject;
class SkeletalMeshComponent;

class EnemyStateBase
{
public:
	EnemyStateBase();

	virtual ~EnemyStateBase() {};

	virtual EnemyState Update(EnemyObject* owner, float deltaTime) = 0;

	virtual void Enter(EnemyObject* owner, float deltaTime) {};

protected:
	// �X�e�[�g�̃^�C�}�[
	float mStateTimer;

	// �U��������ǐՂɕς�鋗��
	const float retrackingRange = 300.0f;

	// �X�P���^�����b�V���N���X�̃|�C���^
	SkeletalMeshComponent* mSkelComp;
 };