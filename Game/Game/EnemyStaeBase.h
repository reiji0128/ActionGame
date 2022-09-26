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

	// �X�P���^�����b�V���N���X�̃|�C���^
	SkeletalMeshComponent* mSkelComp;
 };