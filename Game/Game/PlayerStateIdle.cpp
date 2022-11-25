#include "PlayerStateIdle.h"
#include "Input.h"

PlayerStateIdle::PlayerStateIdle()
{
}

PlayerStateIdle::~PlayerStateIdle()
{
}

/// <summary>
/// 更新処理
/// </summary>
/// <param name="owner">プレイヤーのポインタ</param>
/// <param name="deltaTime">1フレームの経過時間</param>
/// <returns></returns>
PlayerState PlayerStateIdle::Update(PlayerObject* owner, float deltaTime)
{
    // 入力処理
    InputProcess();

    // 移動入力がされたら移動状態に移行
    if (!IsIdle)
    {
        return PlayerState::STATE_RUN;
    }

    // 攻撃入力がされたら攻撃状態に移行
    if (IsAttack)
    {
        return PlayerState::STATE_FIRST_ATTACK;
    }

    // ダメージを受けていたらダメージ受ける状態に移行
    if (owner->GetIsDamage())
    {
        return PlayerState::STATE_FLYING_BACK;
    }

    return PlayerState::STATE_IDLE;
}

/// <summary>
/// 状態が移行したときに1度だけ処理
/// </summary>
/// <param name="owner">プレイヤーのポインタ</param>
/// <param name="deltaTime">1フレームの経過時間</param>
void PlayerStateIdle::Enter(PlayerObject* owner, float deltaTime)
{
    // アイドル状態のアニメーションを再生
    mSkelComp = owner->GetSkeltalMeshComp();
    mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_IDLE));
}

/// <summary>
/// 入力処理
/// </summary>
void PlayerStateIdle::InputProcess()
{
    // コントローラ左スティックが入力されたか
    IsControllerInputOff = !(INPUT_INSTANCE.IsLStickMove());

    // 移動入力がされていないか
    IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_W) &
             INPUT_INSTANCE.IsKeyOff(KEY_S) &
             INPUT_INSTANCE.IsKeyOff(KEY_A) &
             INPUT_INSTANCE.IsKeyOff(KEY_D) &
             IsControllerInputOff;

    // 攻撃入力がされたか
    IsAttack = INPUT_INSTANCE.IsKeyPressed(BUTTON_B);

    // コントローラ入力されたか
    Vector2 stickL = INPUT_INSTANCE.GetLStick();

    // 二つのベクトルから角度(弧度法)を計算
    float angle = Vector2::Angle(stickL, Vector2(0, -1));
    if (stickL.x < 0)
    {
        angle *= -1;
    }
}
