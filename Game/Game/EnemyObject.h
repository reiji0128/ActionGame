#pragma once
#include "GameObject.h"
#include "IHitPointGetter.h"
#include <functional>
#include "ShaderTag.h"
#include <unordered_map>

// �O���錾
class IHitPointGetter;
class EnemyStateBase;
class ColliderComponent;
class SkeletalMeshComponent;
class PointLightComponent;
class HitEffect;
class Animation;
class BoxCollider;
class LineCollider;
class FireBallObject;
enum class DamageType;

enum class EnemyState
{
	STATE_DETECTION,
	STATE_IDLE,
	STATE_PATROL,
	STATE_WALK,
	STATE_RUN,
	STATE_PRELIMINARY_ACTION_HAND,
	STATE_PRELIMINARY_ACTION_BITE,
	STATE_ATTACK_HAND,
	STATE_ATTACK_BITE,
	STATE_FIRE_BREATH,
	STATE_FIRE_BALL,
	STATE_DAMAGE,
	STATE_DEATH,

	STATE_NUM
};

class EnemyObject : public GameObject, public IHitPointGetter
{
public:

	EnemyObject(const Vector3& pos, const float& scale, const char* gpmeshFileName, const char* gpskelFileName);

	~EnemyObject();

	/// <summary>
	/// �Q�[���I�u�W�F�N�g�̍X�V����
	/// </summary>
	/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
	void UpdateGameObject(float deltaTime) override;

	/// <summary>
	/// �Փˏ���
	/// </summary>
	/// <param name="ownCollider">�����̃R���C�_�[</param>
	/// <param name="otherCollider">����̃R���C�_�[</param>
	void OnCollisionEnter(ColliderComponent* ownCollider, ColliderComponent* otherCollider) override;

	void Damage(int damage);

	/// <summary>
	/// �{�[��������{�[���s���T������
	/// </summary>
	/// <param name="boneName">�T���{�[���s��</param>
	/// <returns>�{�[���s��</returns>
	Matrix4 FindBoneMatrixFromName(const char* boneName);

	/// <summary>
	/// ���C�g�̐F�̕ύX����
	/// </summary>
	/// <param name="color"></param>
	void ChangeLightColor(Vector3 color);

// �Z�b�^�[ //

	/// <summary>
	/// �����x�̃Z�b�g
	/// </summary>
	/// <param name="velosity">�����x</param>
	void SetVelosity(const Vector3& velosity) { mVelocity = velosity; }

	/// <summary>
	/// �_���[�W�t���O�̃Z�b�g
	/// </summary>
	/// <param name="isDamage"></param>
	void SetIsDamage(const bool isDamage) { mIsDamage = isDamage; }

	/// <summary>
	/// �o�b�O�O���E���h�t���O�̃Z�b�g
	/// </summary>
	/// <param name="isBackGround"></param>
	void SetIsBackGround(const bool isBackGround) { mIsBackGroundTrigger = isBackGround; }

// �Q�b�^�[ //

	/// <summary>
	/// HP�̎擾
	/// </summary>
	/// <returns>���݂�HP</returns>
	int GetHP() const override { return mHP; }

	/// <summary>
	/// �ő�HP�̎擾
	/// </summary>
	/// <returns>�ő�HP</returns>
	int GetMaxHP() const override { return mMaxHP; }

	int GetReceiveDamage() const { return mDamage; }

	/// <summary>
	/// �_���[�W���󂯂Ă��邩�̎擾
	/// </summary>
	/// <returns></returns>
	bool GetIsDamage() const { return mIsDamage; }

	/// <summary>
	/// �o�b�O�O���E���h�Ɠ������Ă��邩�ǂ����̎擾
	/// </summary>
	/// <returns></returns>
	bool GetIsBackGround() const { return mIsBackGroundTrigger; }

	/// <summary>
	/// �X�P���^�����b�V���R���|�[�l���g�̃|�C���^�̎擾
	/// </summary>
	/// <returns>�X�P���^�����b�V���R���|�[�l���g�̃|�C���^</returns>
	SkeletalMeshComponent* GetSkeletalMeshComp() { return mSkeltalMeshComp; }

	/// <summary>
	/// �A�j���[�V�����̎擾
	/// </summary>
	/// <param name="state">�X�e�[�g�ɑΉ�����A�j���[�V����</param>
	/// <returns></returns>
	const Animation* GetAnim(EnemyState state) const { return mAnimations[static_cast<unsigned int>(state)]; }

	int GetDamage(DamageType type) const;

private:

	// �̗�
	int mHP;

	// �ő�̗�
	const int mMaxHP = 100;

	int mDamage;

	// ���G���Ԃ̃^�C�}�[
	float mEnvincibleTimer;

	// ���G����
	const float mEnvincibleTime;

	// �_���[�W���󂯂Ă��邩
	bool mIsDamage;

	// ���G���ǂ���
	bool mIsEnvincible;

	// ���G���Ԃ̃^�C�}�[���X�^�[�g���邩
	bool mStartEnvincibleTimer;

	// �o�b�O�O���E���h�Ɠ������Ă��邩
	bool mIsBackGroundTrigger;

	// �����x
	Vector3 mVelocity;

	// ���݂̏��
	EnemyState mNowState;

	// ���̏��
	EnemyState mNextState;

	// �U�����X�g
	std::unordered_map<DamageType, int> mAttackList;

	// �A�j���[�V�����̃R���e�i
	std::vector<const Animation*> mAnimations;

	// ��Ԃ̃v�[��
	std::vector<EnemyStateBase*> mStatePools;

	// �X�P���^�����b�V���R���|�W�b�g�̃|�C���^
	SkeletalMeshComponent* mSkeltalMeshComp;

	// �����蔻��p�{�b�N�X
	BoxCollider* mBoxCollider;

	// �w�i�̔���g���K�[�{�b�N�X
	BoxCollider* mBackGroundTrigger;

	// �����𒲂ׂ郉�C���R���C�_�[
	LineCollider* mLineCollider;

	// PointLightComponent�̃|�C���^
	PointLightComponent* mCenterLight;
	PointLightComponent* mBehindLight;
	PointLightComponent* mFrontLight;
	PointLightComponent* mLeftLight;
	PointLightComponent* mRightLight;
	PointLightComponent* mFrontLeftLight;
	PointLightComponent* mFrontRightLight;

	// EffectComponent�̃|�C���^
	HitEffect* mHitEffect;

	ShaderTag mUseShaderTag;
};