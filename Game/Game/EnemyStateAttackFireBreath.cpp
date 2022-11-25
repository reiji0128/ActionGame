#include "EnemyStateAttackFireBreath.h"
#include "EnemyObject.h"
#include "Game.h"
#include "AudioManager.h"
#include "SkeletalMeshComponent.h"
#include "EffectComponent.h"
#include "AttackObj.h"
#include "FireBreathObject.h"

EnemyStateAttackFireBreath::EnemyStateAttackFireBreath()
{
}

EnemyStateAttackFireBreath::~EnemyStateAttackFireBreath()
{
}

EnemyState EnemyStateAttackFireBreath::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;
	Matrix4 animMatrix;

	// HP��0�ɂȂ����玀�S��ԂɈڍs
	if (owner->GetHP() <= 0)
	{
		mFireBreath->SetState(GameObject::EDead);
		return EnemyState::STATE_DEATH;
	}

//	RotateProcess(owner, deltaTime);

	// ���̍s���Vector3�ɕϊ�
	animMatrix = owner->FindBoneMatrixFromName("MOUNTAIN_DRAGON_ Head");
	mMouthPos = Vector3::Transform(Vector3::Zero, animMatrix);
	mMouthPos.z = 230;
	mFireBreath->SetPosition(mMouthPos);

	// �G����v���C���[�Ɍ������x�N�g�����Z�o
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();

	// �A�j���[�V�����̍Đ��͏I�������
	if (!mSkelComp->IsPlaying())
	{
		mFireBreath->SetState(GameObject::EDead);
		owner->SetDirection(enemyToPlayerVec);
		// �v���C���[�Ƃ̋������ǐՂ��鋗�������Z����
		if (length < mNearLength)
		{
			return EnemyState::STATE_IDLE;
		}
		else
		{
			// �������Z�o
			int probability = rand() % 100;

			// 40%�̊m���łЂ������U���ڍs
			if (0 <= probability && probability < 60)
			{
				return EnemyState::STATE_WALK;
			}

			if (70 <= probability && probability < 100)
			{
				return EnemyState::STATE_RUN;
			}
		}
	}

	return EnemyState::STATE_FIRE_BREATH;
}

void EnemyStateAttackFireBreath::Enter(EnemyObject* owner, float deltaTime)
{
	// �A�j���[�V�����̍Đ�
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_FIRE_BREATH),0.3f);

	// �X�e�[�g�^�C�}�[�̏�����
	mStateTimer = 3.0f;

	// �v���C���[���W�̎擾
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// �G�̍��W�̎擾
	Vector3 ownerPos = owner->GetPosition();
	// �G����v���C���[�Ɍ������x�N�g���̒������Z�o
	mEnemyToPlayerVec = playerPos - ownerPos;
	float length = mEnemyToPlayerVec.Length();
	mEnemyToPlayerVec.z = 0.0f;
	mEnemyToPlayerVec.Normalize();
	//owner->SetDirection(mEnemyToPlayerVec);

	Vector3 velosity = Vector3(0, 0, 0);
	owner->SetVelosity(velosity);

	AttackProcess(owner, deltaTime);

	// �v���C���[���W�̎擾
	playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// �G�̍��W�̎擾
	ownerPos = owner->GetPosition();
	// �G����v���C���[�Ɍ������x�N�g�����Z�o
	mAimVec = playerPos - ownerPos;
	mAimVec.z = 0.0f;
	mAimVec.Normalize();

	mRotateNow = true;

	//owner->ChangeLightColor(Vector3(1,0,0));
}

void EnemyStateAttackFireBreath::AttackProcess(EnemyObject* owner, float deltaTime)
{
	Matrix4 animMatrix;

	// ���̍s���Vector3�ɕϊ�
	animMatrix = owner->FindBoneMatrixFromName("MOUNTAIN_DRAGON_ Head");
	mMouthPos = Vector3::Transform(Vector3::Zero, animMatrix);
	mMouthPos.y += 100;
	mMouthPos.z = 200;

	// �u���X�̐���
	mFireBreath = new FireBreathObject(mMouthPos, owner->GetDirection(),Tag::EnemyFireBreath);

	AUDIO->PlaySoundW("Assets/Sound/FireBreath.wav");

}
