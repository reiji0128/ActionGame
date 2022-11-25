#include "PlayerStateIdle.h"
#include "Input.h"

PlayerStateIdle::PlayerStateIdle()
{
}

PlayerStateIdle::~PlayerStateIdle()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="owner">�v���C���[�̃|�C���^</param>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
/// <returns></returns>
PlayerState PlayerStateIdle::Update(PlayerObject* owner, float deltaTime)
{
    // ���͏���
    InputProcess();

    // �ړ����͂����ꂽ��ړ���ԂɈڍs
    if (!IsIdle)
    {
        return PlayerState::STATE_RUN;
    }

    // �U�����͂����ꂽ��U����ԂɈڍs
    if (IsAttack)
    {
        return PlayerState::STATE_FIRST_ATTACK;
    }

    // �_���[�W���󂯂Ă�����_���[�W�󂯂��ԂɈڍs
    if (owner->GetIsDamage())
    {
        return PlayerState::STATE_FLYING_BACK;
    }

    return PlayerState::STATE_IDLE;
}

/// <summary>
/// ��Ԃ��ڍs�����Ƃ���1�x��������
/// </summary>
/// <param name="owner">�v���C���[�̃|�C���^</param>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
void PlayerStateIdle::Enter(PlayerObject* owner, float deltaTime)
{
    // �A�C�h����Ԃ̃A�j���[�V�������Đ�
    mSkelComp = owner->GetSkeltalMeshComp();
    mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_IDLE));
}

/// <summary>
/// ���͏���
/// </summary>
void PlayerStateIdle::InputProcess()
{
    // �R���g���[�����X�e�B�b�N�����͂��ꂽ��
    IsControllerInputOff = !(INPUT_INSTANCE.IsLStickMove());

    // �ړ����͂�����Ă��Ȃ���
    IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_W) &
             INPUT_INSTANCE.IsKeyOff(KEY_S) &
             INPUT_INSTANCE.IsKeyOff(KEY_A) &
             INPUT_INSTANCE.IsKeyOff(KEY_D) &
             IsControllerInputOff;

    // �U�����͂����ꂽ��
    IsAttack = INPUT_INSTANCE.IsKeyPressed(BUTTON_B);

    // �R���g���[�����͂��ꂽ��
    Vector2 stickL = INPUT_INSTANCE.GetLStick();

    // ��̃x�N�g������p�x(�ʓx�@)���v�Z
    float angle = Vector2::Angle(stickL, Vector2(0, -1));
    if (stickL.x < 0)
    {
        angle *= -1;
    }
}
