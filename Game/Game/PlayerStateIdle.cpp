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
    // �R���g���[�����͂��ꂽ��
    bool isControllerInputoff = !(INPUT_INSTANCE.IsLStickMove());

    // �����L�[�����͂��ꂽ��
    bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_W) &
                  INPUT_INSTANCE.IsKeyOff(KEY_S) &
                  INPUT_INSTANCE.IsKeyOff(KEY_A) &
                  INPUT_INSTANCE.IsKeyOff(KEY_D) &
                  isControllerInputoff;

    // �ړ����͂����ꂽ��
    if (!IsIdle)
    {
        return PlayerState::STATE_RUN;
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
