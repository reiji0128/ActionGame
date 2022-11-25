#include "PlayerStateRun.h"
#include "Input.h"

/// <summary>
/// コンストラクタ
/// </summary>
PlayerStateRun::PlayerStateRun()
	:mRotationTargetDir(0.0f,0.0f,0.0f)
{
	IsRollForward;
}

/// <summary>
/// デストラクタ
/// </summary>
PlayerStateRun::~PlayerStateRun()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="owner">プレイヤーのポインタ</param>
/// <param name="deltaTime">1フレームの経過時間</param>
/// <returns></returns>
PlayerState PlayerStateRun::Update(PlayerObject* owner, float deltaTime)
{
	// 入力処理
	InputProcess();

	// 移動処理
	MoveCalc(owner,deltaTime);

	// 移動入力がなかったらアイドル状態に移行
	if (IsIdle)
	{
		return PlayerState::STATE_IDLE;
	}

	if (IsAttack)
	{
		return PlayerState::STATE_FIRST_ATTACK;
	}

	if (IsRollForward)
	{
		IsRollForward = false;
		return PlayerState::STATE_ROLL_FORWARD;
	}

	// ダメージを受けていたらダメージ受ける状態に移行
	if (owner->GetIsDamage())
	{
		return PlayerState::STATE_FLYING_BACK;
	}

	return PlayerState::STATE_RUN;
}

/// <summary>
/// 状態が移行したときに1度だけ処理
/// </summary>
/// <param name="owner">プレイヤーのポインタ</param>
/// <param name="deltaTime">1フレームの経過時間</param>
void PlayerStateRun::Enter(PlayerObject* owner, float deltaTime)
{
    // アイドル状態のアニメーションを再生
    mSkelComp = owner->GetSkeltalMeshComp();
    mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_RUN),0.6f);

	IsRollForward = false;
}

void PlayerStateRun::InputProcess()
{

	// コントローラ左スティックが入力されたか
	IsControllerInputOff = !INPUT_INSTANCE.IsLStickMove();

	// 移動入力がされていないか
	IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_W) &
		     INPUT_INSTANCE.IsKeyOff(KEY_S) &
		     INPUT_INSTANCE.IsKeyOff(KEY_D) &
		     INPUT_INSTANCE.IsKeyOff(KEY_A) &
		     IsControllerInputOff;

	// 攻撃入力がされたか
	IsAttack = INPUT_INSTANCE.IsKeyPressed(BUTTON_B);

	// 前転の入力があったか
	IsRollForward = INPUT_INSTANCE.IsKeyPressed(BUTTON_A);
}

/// <summary>
/// 移動処理
/// </summary>
/// <param name="owner">プレイヤーのポインタ</param>
/// <param name="deltaTime">1フレームの経過時間</param>
void PlayerStateRun::MoveCalc(PlayerObject* owner, float deltaTime)
{
	// スピード
	const float speed = 200.0f;
	// 現在のプレイヤーのスピード
	float charaSpeed = owner->GetSpeed();

    // カメラからプレイヤーに向かうベクトルを算出
	Vector3 playerPos = GAMEINSTANCE.GetViewTargetPos();
	Vector3 cameraPos = GAMEINSTANCE.GetViewPos();
    Vector3 forwardVec = playerPos - cameraPos;

    forwardVec.z = 0.0f;
    forwardVec = Vector3::Normalize(forwardVec);

    // 前方ベクトルとz軸から右方向ベクトルを算出
    Vector3 rightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// 右方向から回転角を算出
	float forwardAngle = 0.0f;
	float angleSign;
	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, rightVec));

	// 右回転か左回転か？
	tmpVec = Vector3::Cross(Vector3::UnitX, rightVec);
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;

	// プレイヤーの前方ベクトル
	mCharaForwardVec = owner->GetForward();

// キーボード入力 //
    Vector3 DirVec(0.0f, 0.0f, 0.0f);
	if (INPUT_INSTANCE.IsKeyPressed(KEY_W))
	{
		DirVec += forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_S))
	{
		DirVec -= forwardVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_D))
	{
		DirVec += rightVec;
	}

	if (INPUT_INSTANCE.IsKeyPressed(KEY_A))
	{
		DirVec -= rightVec;
	}

// ゲームパッド入力 //
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	stickL = Vector2::Transform(stickL, rot);
	DirVec.x += stickL.x;
	DirVec.y += stickL.y;

	if (DirVec.Length() > 0.5)
	{
		
		DirVec.Normalize();

	    // 回転中か？
		if (mRotateNow)
		{
			// 回転処理
			RotateActor();
		}

	    // 入力とプレイヤーの前方ベクトルがほぼ同じなら
		if (isNearAngle(DirVec, mCharaForwardVec))
		{
			mCharaForwardVec = DirVec;
		}
		else
		{
			mRotateNow = true;
			mRotationTargetDir = DirVec;
		}
		
		// 現在のスピードを保存
		charaSpeed = speed * deltaTime;
	}

	// 移動処理
	DirVec.z = 0;
	Vector3 velocity;
	velocity += charaSpeed * DirVec;

	// 最高速度を超えていたら調整
	Vector3 horizonSpeed = velocity;
	if (horizonSpeed.Length() > 200 * deltaTime)
	{
		horizonSpeed = DirVec * 200 * deltaTime;
		velocity.x = horizonSpeed.x;
		velocity.y = horizonSpeed.y;
	}

	// プレイヤーの位置・スピード・
	// 前方ベクトル・変換行列の再計算の必要をセット
	//owner->SetPosition(playerPos);
	owner->SetSpeed(charaSpeed);
	owner->SetVelocity(velocity);
	owner->SetDirection(mCharaForwardVec);
	owner->SetComputeWorldTransform();
}

/// <summary>
/// 回転処理
/// </summary>
void PlayerStateRun::RotateActor()
{
	// 前方ベクトルが目標方向に近ければ回転終了
	if (isNearAngle(mRotationTargetDir, mCharaForwardVec))
	{
		mCharaForwardVec = mRotationTargetDir;
		mRotateNow = false;
	}
	else
	{
		// 回転させる
		Vector3 calcVec;
		calcVec = zRotateToAimVec(mCharaForwardVec, mRotationTargetDir, 50.0f);

		// 前方ベクトルが目標方向を超えていないか
		Vector3 cross1, cross2;
		cross1 = Vector3::Cross(mCharaForwardVec, mRotationTargetDir);
		cross2 = Vector3::Cross(calcVec, mRotationTargetDir);

		// 目標方向を超えたら終了
		if (cross1.z * cross2.z < 0.0f)
		{
			calcVec = mRotationTargetDir;
			mRotateNow = false;
		}
		mCharaForwardVec = calcVec;
	}
}
