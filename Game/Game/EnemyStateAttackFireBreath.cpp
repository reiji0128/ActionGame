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

	// HPが0になったら死亡状態に移行
	if (owner->GetHP() <= 0)
	{
		mFireBreath->SetState(GameObject::EDead);
		return EnemyState::STATE_DEATH;
	}

//	RotateProcess(owner, deltaTime);

	// 口の行列をVector3に変換
	animMatrix = owner->FindBoneMatrixFromName("MOUNTAIN_DRAGON_ Head");
	mMouthPos = Vector3::Transform(Vector3::Zero, animMatrix);
	mMouthPos.z = 230;
	mFireBreath->SetPosition(mMouthPos);

	// 敵からプレイヤーに向かうベクトルを算出
	Vector3 playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	Vector3 enemyToPlayerVec = playerPos - owner->GetPosition();

	float length = enemyToPlayerVec.Length();

	// アニメーションの再生は終わったか
	if (!mSkelComp->IsPlaying())
	{
		mFireBreath->SetState(GameObject::EDead);
		owner->SetDirection(enemyToPlayerVec);
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

	return EnemyState::STATE_FIRE_BREATH;
}

void EnemyStateAttackFireBreath::Enter(EnemyObject* owner, float deltaTime)
{
	// アニメーションの再生
	mSkelComp = owner->GetSkeletalMeshComp();
	mSkelComp->PlayAnimation(owner->GetAnim(EnemyState::STATE_FIRE_BREATH),0.3f);

	// ステートタイマーの初期化
	mStateTimer = 3.0f;

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

	Vector3 velosity = Vector3(0, 0, 0);
	owner->SetVelosity(velosity);

	AttackProcess(owner, deltaTime);

	// プレイヤー座標の取得
	playerPos = GAMEINSTANCE.GetPlayerObjects()->GetPosition();
	// 敵の座標の取得
	ownerPos = owner->GetPosition();
	// 敵からプレイヤーに向かうベクトルを算出
	mAimVec = playerPos - ownerPos;
	mAimVec.z = 0.0f;
	mAimVec.Normalize();

	mRotateNow = true;

	//owner->ChangeLightColor(Vector3(1,0,0));
}

void EnemyStateAttackFireBreath::AttackProcess(EnemyObject* owner, float deltaTime)
{
	Matrix4 animMatrix;

	// 口の行列をVector3に変換
	animMatrix = owner->FindBoneMatrixFromName("MOUNTAIN_DRAGON_ Head");
	mMouthPos = Vector3::Transform(Vector3::Zero, animMatrix);
	mMouthPos.y += 100;
	mMouthPos.z = 200;

	// ブレスの生成
	mFireBreath = new FireBreathObject(mMouthPos, owner->GetDirection(),Tag::EnemyFireBreath);

	AUDIO->PlaySoundW("Assets/Sound/FireBreath.wav");

}
