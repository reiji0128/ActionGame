#include "PlayerStateRun.h"
#include "Input.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
PlayerStateRun::PlayerStateRun()
	:mRotationTargetDir(0.0f,0.0f,0.0f)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
PlayerStateRun::~PlayerStateRun()
{
}

/// <summary>
/// �X�V����
/// </summary>
/// <param name="owner">�v���C���[�̃|�C���^</param>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
/// <returns></returns>
PlayerState PlayerStateRun::Update(PlayerObject* owner, float deltaTime)
{
	// �R���g���[�����͂��ꂽ��
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	bool isContollerInputOff = !INPUT_INSTANCE.IsLStickMove();

	bool IsIdle = INPUT_INSTANCE.IsKeyOff(KEY_W) &
		          INPUT_INSTANCE.IsKeyOff(KEY_S) &
		          INPUT_INSTANCE.IsKeyOff(KEY_D) &
		          INPUT_INSTANCE.IsKeyOff(KEY_A) &
		          isContollerInputOff;

	// �ړ����͂��Ȃ�������
	if (IsIdle)
	{
		return PlayerState::STATE_IDLE;
	}

	// �ړ�����
	MoveCalc(owner,deltaTime);

	return PlayerState::STATE_RUN;
}

/// <summary>
/// ��Ԃ��ڍs�����Ƃ���1�x��������
/// </summary>
/// <param name="owner">�v���C���[�̃|�C���^</param>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
void PlayerStateRun::Enter(PlayerObject* owner, float deltaTime)
{
    // �A�C�h����Ԃ̃A�j���[�V�������Đ�
    mSkelComp = owner->GetSkeltalMeshComp();
    mSkelComp->PlayAnimation(owner->GetAnim(PlayerState::STATE_RUN));
}

/// <summary>
/// �ړ�����
/// </summary>
/// <param name="owner">�v���C���[�̃|�C���^</param>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
void PlayerStateRun::MoveCalc(PlayerObject* owner, float deltaTime)
{
	// �X�s�[�h
	const float speed = 200.0f;
	// ���݂̃v���C���[�̃X�s�[�h
	float charaSpeed = owner->GetSpeed();

    // �J�������猩���O���x�N�g�����擾
	Vector3 playerPos = GAMEINSTANCE.GetViewTargetPos();
	Vector3 cameraPos = GAMEINSTANCE.GetViewPos();
    Vector3 forwardVec = playerPos - cameraPos;

    forwardVec.z = 0.0f;
    forwardVec = Vector3::Normalize(forwardVec);

    // �O���x�N�g����z������E�����x�N�g�����Z�o
    Vector3 rightVec = Vector3::Cross(Vector3::UnitZ, forwardVec);

	// �E���������]�p���Z�o
	float forwardAngle = 0.0f;
	float angleSign;
	Vector3 tmpVec;
	forwardAngle = acosf(Vector3::Dot(Vector3::UnitX, rightVec));

	// �E��]������]���H
	tmpVec = Vector3::Cross(Vector3::UnitX, rightVec);
	angleSign = (tmpVec.z > 0.0) ? 1.0f : -1.0f;
	forwardAngle *= angleSign;

	// �v���C���[�̑O���x�N�g��
	mCharaForwardVec = owner->GetForward();

    // �L�[����
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

	// �Q�[���p�b�h
	Vector2 stickL = INPUT_INSTANCE.GetLStick();
	Matrix3 rot = Matrix3::CreateRotation(forwardAngle);
	stickL = Vector2::Transform(stickL, rot);
	DirVec.x += stickL.x;
	DirVec.y += stickL.y;

	if (DirVec.Length() > 0.5)
	{
		
		DirVec.Normalize();

	    // ��]�����H
		if (mRotateNow)
		{
			RotateActor();
		}

	    // ���͂ƃv���C���[�̑O���x�N�g�����قړ����Ȃ�
		if (isNearAngle(DirVec, mCharaForwardVec))
		{
			mCharaForwardVec = DirVec;
		}
		else
		{
			mRotateNow = true;
			mRotationTargetDir = DirVec;
		}
		
		// ���݂̃X�s�[�h��ۑ�
		charaSpeed = speed * deltaTime;
	}

	// �ړ�����
	playerPos += charaSpeed * DirVec;

	// �v���C���[�̈ʒu�E�X�s�[�h�E
	// �O���x�N�g���E�ϊ��s��̍Čv�Z�̕K�v���Z�b�g
	owner->SetPosition(playerPos);
	owner->SetSpeed(charaSpeed);
	owner->SetDirection(mCharaForwardVec);
	owner->SetComputeWorldTransform();
}

/// <summary>
/// ��]����
/// </summary>
void PlayerStateRun::RotateActor()
{
	// �O���x�N�g�����ڕW�����ɋ߂���Ή�]�I��
	if (isNearAngle(mRotationTargetDir, mCharaForwardVec))
	{
		mCharaForwardVec = mRotationTargetDir;
		mRotateNow = false;
	}
	else
	{
		// ��]������
		Vector3 calcVec;
		calcVec = zRotateToAimVec(mCharaForwardVec, mRotationTargetDir, 10.0f);

		// �O���x�N�g�����ڕW�����𒴂��Ă��Ȃ���
		Vector3 cross1, cross2;
		cross1 = Vector3::Cross(mCharaForwardVec, mRotationTargetDir);
		cross2 = Vector3::Cross(calcVec, mRotationTargetDir);

		// �ڕW�����𒴂�����I��
		if (cross1.z * cross2.z < 0.0f)
		{
			calcVec = mRotationTargetDir;
			mRotateNow = false;
		}
		mCharaForwardVec = calcVec;
	}
}
