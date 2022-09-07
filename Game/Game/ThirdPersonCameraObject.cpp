#include "ThirdPersonCameraObject.h"
#include "Game.h"
#include "Renderer.h"
#include "Input.h"

const float cameraLength = 1000.0f;
const float cameraHight = 350.0f;
const float rotateAngle = -Math::Pi;
const float upAngle = Math::PiOver2 * 0.5f;
const float rotateSpeed = 40.0f;

// �ő�p
const float maxLookDownAngle = Math::ToRadians(70.0f);
// �ŏ��p
const float minLookDownAngle = Math::ToRadians(20.0f);

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="targetObject">�J��������Ƃ���Q�[���I�u�W�F�N�g�̃|�C���^</param>
ThirdPersonCameraObject::ThirdPersonCameraObject(GameObject* targetObject)
	:CameraObject(targetObject)
	,mCameraLength(0.0f)
	,mRotateZAngle(0.0f)
	,mLookDownAngle()
{
	mTargetDir = targetObject->GetDirection();
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
ThirdPersonCameraObject::~ThirdPersonCameraObject()
{
}

/// <summary>
/// �Q�[���I�u�W�F�N�g�̍X�V����
/// </summary>
/// <param name="deltaTime">1�t���[���̌o�ߎ���</param>
void ThirdPersonCameraObject::UpdateGameObject(float deltaTime)
{
	// �v���C���[�����݂��Ȃ��ꍇ�͏������Ȃ�
	if (!GAMEINSTANCE.IsExistGameObjectType(Tag::Player))
	{
		return;
	}

	// �ڋߌW��
	const float dumper = 1.5f;
	Vector3 diff;
	Vector3 playerPos = mTargetActor->GetPosition();

	// �����x�N�g������v���C���[�ʒu�܂ŋ߂Â���
	diff = playerPos - mViewTarget;
	diff = dumper * deltaTime * diff;

	// �L�[����
	const float rotate = 0.5f * deltaTime;
	if (INPUT_INSTANCE.IsKeyPressed(KEY_RIGHT))
	{
		mRotateZAngle += rotate;
	}
	if (INPUT_INSTANCE.IsKeyPressed(KEY_LEFT))
	{
		mRotateZAngle -= rotate;
	}
	if (INPUT_INSTANCE.IsKeyPressed(KEY_UP))
	{
		mLookDownAngle += rotate;
	}
	if (INPUT_INSTANCE.IsKeyPressed(KEY_DOWN))
	{
		mLookDownAngle -= rotate;
	}

	// �p�b�h����
	Vector2 axisR;
	axisR = INPUT_INSTANCE.GetRStick();
	mRotateZAngle += axisR.x * rotate;
	mLookDownAngle += axisR.y * rotate;
	// ���~�낵�p�x�̊p�x����
	if (mLookDownAngle < minLookDownAngle)
	{
		mLookDownAngle = minLookDownAngle;
	}
	if (mLookDownAngle > maxLookDownAngle)
	{
		mLookDownAngle = maxLookDownAngle;
	}

	// ���[��]�E�s�b�`��]
	Vector3 rotatePos;
	rotatePos.x = -mTargetDir.x * mCameraLength * cosf(mLookDownAngle) * cosf(mRotateZAngle);
	rotatePos.y = mCameraLength * cosf(mLookDownAngle) * sinf(mRotateZAngle);
	rotatePos.z = mCameraLength * sinf(mLookDownAngle);

	// �����_�E�J�����ʒu���Z�b�g
	mPosition = rotatePos + playerPos;
	mViewTarget = playerPos;

	// �J�����s��쐬
	Matrix4 camMat = Matrix4::CreateLookAt(mPosition, mViewTarget, Vector3(0, 0, 1));
	RENDERER->SetViewMatrix(camMat);
}
