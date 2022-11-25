#include "EnemyStateRun.h"
#include "SkeletalMeshComponent.h"
#include "Game.h"
#include "AttackObj.h"

EnemyStateRun::EnemyStateRun()
	:runSpeed(500.0f)
{
}

EnemyState EnemyStateRun::Update(EnemyObject* owner, float deltaTime)
{
	mStateTimer -= deltaTime;

	if (owner->GetHP() <= 0)
	{
		return EnemyState::STATE_DEATH;
	}

	// プレイヤー座標の取得
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// 敵の座標の取得
	Vector3 ownerPos = owner->GetPosition();
	// 敵からプレイヤーに向かうベクトルの長さを算出
	Vector3 enemyToPlayerVec = playerPos - ownerPos;
	float length = enemyToPlayerVec.Length();
	enemyToPlayerVec.Normalize();

	// 敵の向きの取得
	Vector3 dir = owner->GetDirection();

	if (!isNearAngle(owner->GetDirection(), enemyToPlayerVec))
	{
		mRotateNow = true;
	}

	RotateProcess(owner,deltaTime);
	if (!mRotateNow)
	{
		//プレイヤーとの距離が近い場合
		if (InViewAngle(dir, enemyToPlayerVec, length) || mStateTimer < 0.0)
		{
			// 乱数を算出
			int probability = rand() % 100;

			// 40%の確立でひっかき攻撃移行
			if (0 <= probability && probability < 40)
			{
				return EnemyState::STATE_PRELIMINARY_ACTION_HAND;
			}

			// 40%の確立で噛みつき攻撃に移行
			if (40 <= probability && probability < 80)
			{
				return EnemyState::STATE_PRELIMINARY_ACTION_BITE;
			}

			// 10%の確立で火ブレス攻撃に移行
			if (80 <= probability && probability < 90)
			{
				return EnemyState::STATE_FIRE_BREATH;
			}

			// 10%の確立で火球攻撃に移行
			if (90 <= probability && probability < 100)
			{
				return EnemyState::STATE_FIRE_BALL;
			}
		}

		// プレイヤーとの距離が遠い場合
		if (length > mNearLength && mStateTimer < 0.0)
		{
			// 乱数を算出
			int probability = rand() % 100;

			// 30%の確立でひっかき攻撃移行
			if (0 <= probability && probability < 20)
			{
				return EnemyState::STATE_WALK;
			}

			// 35%の確立でブレス攻撃に移行
			if (20 <= probability && probability < 60)
			{
				return EnemyState::STATE_FIRE_BREATH;
			}

			// 35%の確立で火球攻撃に移行
			if (60 <= probability && probability < 100)
			{
				return EnemyState::STATE_FIRE_BALL;
			}
		}

		// 敵からプレイヤーに向かうベクトルを敵の向きにセット
		/*enemyToPlayerVec.z = 0.0f;
		enemyToPlayerVec.Normalize();
		owner->SetDirection(enemyToPlayerVec);*/

		//移動処理
		MoveCalc(owner, deltaTime);
	}

	return EnemyState::STATE_RUN;
}

void EnemyStateRun::Enter(EnemyObject* owner, float deltaTime)
{
	// アニメーションの再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_RUN),0.8f);
	Attack(owner, deltaTime);
	mStateTimer = 4.0f;

	// プレイヤー座標の取得
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// 敵の座標の取得
	Vector3 ownerPos = owner->GetPosition();
	// 敵からプレイヤーに向かうベクトルの長さを算出
	mEnemyToPlayerVec = playerPos - ownerPos;
	float length = mEnemyToPlayerVec.Length();
	mEnemyToPlayerVec.z = 0.0f;
	mEnemyToPlayerVec.Normalize();
	//owner->SetDirection(mEnemyToPlayerVec);
	mPlayerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();

	mRotateNow = true;
}

void EnemyStateRun::MoveCalc(EnemyObject* owner, float deltaTime)
{
	//水平方向の向きを取得
	Vector3 dir = mEnemyToPlayerVec;
	dir.z = 0.0f;
	dir.Normalize();

	Vector3 velocity = dir * runSpeed * deltaTime;

	owner->SetVelosity(velocity);

	owner->SetDirection(dir);
	owner->SetComputeWorldTransform();
}

void EnemyStateRun::RotateProcess(EnemyObject* owner, float deltaTime)
{
		//水平方向の向きを取得
		Vector3 dir = mEnemyToPlayerVec;
		dir.z = 0.0f;
		dir.Normalize();

		Vector3 nowDirection = owner->GetDirection();

		// プレイヤー座標の取得
		Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
		// 敵の座標の取得
		Vector3 ownerPos = owner->GetPosition();
		// 敵からプレイヤーに向かうベクトルの長さを算出
		Vector3 aimDir = mPlayerPos - ownerPos;
		aimDir.Normalize();

		float rotateSpeed = 45.0f;

		if (mRotateNow)
		{
			if (isNearAngle(nowDirection, aimDir))
			{
				owner->SetDirection(nowDirection);
				mRotateNow = false;
				Vector3 velocity = nowDirection * rotateSpeed * deltaTime;
				owner->SetVelosity(velocity);
			}

			// 回転処理
			nowDirection = zRotateToAimVec(nowDirection, aimDir, rotateSpeed * deltaTime);
			owner->SetDirection(nowDirection);
			owner->RotateToNewForward();
		}

		if (owner->GetIsBackGround())
		{
			// 90度時計回りに方向転換＆回転モードに移行
			mRotateNow = true;
			mAimVec = Vector3::Cross(Vector3::UnitZ, nowDirection);
			owner->SetVelosity(Vector3::Zero);
			return;
		}
}

void EnemyStateRun::Attack(EnemyObject* owner, float deltaTime)
{
	if (mStateTimer < 0.5)
	{
		new AttackObj(Tag::EnemyAttackBite, owner->GetPosition(), Vector3(500, 500, 0), owner->GetDirection(), 150.0f, mStateTimer);
		// タイマーを無限大にリセットして攻撃を繰り返さないようにする
		mStateTimer = FLT_MAX;
	}
}
