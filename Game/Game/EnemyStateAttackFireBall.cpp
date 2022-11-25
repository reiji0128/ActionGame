#include "EnemyStateAttackFireBall.h"
#include "Game.h"
#include "AudioManager.h"
#include "SkeletalMeshComponent.h"
#include "AttackObj.h"
#include "FireBallObject.h"

EnemyStateAttackFireBall::EnemyStateAttackFireBall()
{
}

EnemyStateAttackFireBall::~EnemyStateAttackFireBall()
{
}

EnemyState EnemyStateAttackFireBall::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}

	RotateProcess(owner, deltaTime);

	if (mStateTimer <= 0.0f)
	{
		AttackProcess(owner, deltaTime);

		// タイマーを無限大にリセットして攻撃を繰り返さないようにする
		mStateTimer = FLT_MAX;
	}

	// 敵からプレイヤーに向かうベクトルを算出
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();

	// アニメーションの再生は終わったか
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

			// 40%の確立でひっかき攻撃移行
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

	return EnemyState::STATE_FIRE_BALL;
}

void EnemyStateAttackFireBall::Enter(EnemyObject* owner, float deltaTime)
{
	// アニメーションの再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_FIRE_BALL),0.5f);

	// ステートタイマーの初期化
	mStateTimer = 1.0f;

	// プレイヤー座標の取得
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// 敵の座標の取得
	Vector3 ownerPos = owner->GetPosition();
	// 敵からプレイヤーに向かうベクトルの長さを算出
	mEnemyToPlayerVec = playerPos - ownerPos;
	float length = mEnemyToPlayerVec.Length();
	mEnemyToPlayerVec.z = 0.0f;
	mEnemyToPlayerVec.Normalize();

	Vector3 velosity = Vector3(0, 0, 0);
	owner->SetVelosity(velosity);

	// プレイヤー座標の取得
	playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// 敵の座標の取得
	ownerPos = owner->GetPosition();
	// 敵からプレイヤーに向かうベクトルを算出
	mAimVec = playerPos - ownerPos;
	mAimVec.z = 0.0f;
	mAimVec.Normalize();

	mRotateNow = true;

	//owner->ChangeLightColor(Vector3(1, 0, 0));
}

void EnemyStateAttackFireBall::AttackProcess(EnemyObject* owner, float deltaTime)
{
	Matrix4 animMatrix;

	// 口の行列をVector3に変換
	animMatrix =  owner->FindBoneMatrixFromName("MOUNTAIN_DRAGON_ Head");
	mMouthPos = Vector3::Transform(Vector3::Zero, animMatrix);
	mMouthPos.z += 250;

	new FireBallObject(mMouthPos, owner->GetDirection(), 500, Tag::EnemyFireBall);
	AUDIO->PlaySoundW("Assets/Sound/FireBall.wav");

}
