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

	/// <summary>
	/// ��]����
	/// </summary>
	/// <param name="owner">�G�l�~�[�̃|�C���^</param>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void RotateProcess(EnemyObject* owner, float deltaTime);

	/// ����p�ɓ����Ă��邩�ǂ���
	/// </summary>
	/// <param name="from">��̃x�N�g��</param>
	/// <param name="to">�p�x���𑪒肷��x�N�g��</param>
	/// <param name="length">�͈͂̒���</param>
	/// <returns>
	/// true  : ����p�ɓ����Ă���
	/// false : ����p�ɓ����Ă��Ȃ�
	/// </returns> 
	bool InViewAngle(const Vector3& from, const Vector3& to, float length);

	// �X�e�[�g�̃^�C�}�[
	float mStateTimer;

	// �U��������ǐՂɕς�鋗��
	const float retrackingRange = 600.0f;

	// �v���C���[�Ƃ̋���(�ߋ���)
	const float mNearLength = 600.0f;

	// �v���C���[�Ƃ̋���(������)
	const float mFarLength = 800.0f;

	// ����p
	const float viewAngle = 45;

	// �U���A�j���J�n���Ă���U�����肪��������܂ł̒�������
	const float attackOffsetTime = 0.5f;

	// ��]�X�s�[�h
	const float mRotateSpeed = 45.0f;

	// ��]���Ă��邩
	bool mRotateNow;

	// �ڕW�ւ̃x�N�g��
	Vector3 mAimVec;

	// �X�P���^�����b�V���N���X�̃|�C���^
	SkeletalMeshComponent* mSkelComp;
 };