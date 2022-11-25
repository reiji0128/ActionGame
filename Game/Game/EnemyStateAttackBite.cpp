#include "EnemyStateAttackBite.h"
#include "EnemyObject.h"
#include "Game.h"
#include "SkeletalMeshComponent.h"
#include "AttackObj.h"

EnemyStateAttackBite::EnemyStateAttackBite()
{
}

EnemyStateAttackBite::~EnemyStateAttackBite()
{
}

EnemyState EnemyStateAttackBite::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}

	RotateProcess(owner, deltaTime);

	// 敵からプレイヤーに向かうベクトルを算出
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();

	mAimVec = enemyToPlayerVec;
	mAimVec.z = 0.0f;
	mAimVec.Normalize();

	// アニメーションは再生中か
	if (!mSkelComp->IsPlaying())
	{
		// プレイヤーとの距離が追跡する距離よりも短いか
		if (length < mNearLength)
		{
			return EnemyState::STATE_IDLE;
		}
		else
		{
			// 乱数を算出
			int probability = rand() % 100;

			// 30%の確立でひっかき攻撃移行
			if (0 <= probability && probability < 40)
			{
				return EnemyState::STATE_WALK;
			}

			if (40 <= probability && probability < 100)
			{
				return EnemyState::STATE_RUN;
			}
		}
	}

	// 攻撃処理
	Attack(owner, deltaTime);

	enemyToPlayerVec.z = 0.0f;
	enemyToPlayerVec.Normalize();

	return EnemyState::STATE_ATTACK_BITE;
}

void EnemyStateAttackBite::Enter(EnemyObject* owner, float deltaTime)
{
	// アニメーションの再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_ATTACK_BITE),0.3f);

	mStateTimer = attackOffsetTime;

	Vector3 velosity = velosity.Zero;
	owner->SetVelosity(velosity);

	// プレイヤー座標の取得
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();

	// 敵の座標の取得
	Vector3 ownerPos = owner->GetPosition();

	// 敵からプレイヤーに向かうベクトルを算出
	mAimVec = playerPos - ownerPos;
	mAimVec.z = 0.0f;
	mAimVec.Normalize();

	mRotateNow = true;

	//owner->ChangeLightColor(Vector3(0, 1, 0));
}

void EnemyStateAttackBite::Attack(EnemyObject* owner, float deltaTime)
{
	Vector3 ownerPos = owner->GetPosition();
	mAttackObjOffset = Vector3(-100, 0, 0);
	Vector3 attackObjPos = ownerPos + mAttackObjOffset;

	if (mStateTimer < 0.0)
	{
		new AttackObj(Tag::EnemyAttackBite, owner->GetPosition(), Vector3(300, 300, 0), owner->GetDirection(), 300.0f, 1.0f);

		// タイマーを無限大にリセットして攻撃を繰り返さないようにする
		mStateTimer = FLT_MAX;
	}

}
