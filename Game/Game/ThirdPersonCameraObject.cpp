#include "ThirdPersonCameraObject.h"
#include "Game.h"
#include "PhysicsWorld.h"
#include "Renderer.h"
#include "Input.h"
#include "Collision.h"
#include "TargetSearch.h"

const float rotateAngle = -Math::Pi;
const float upAngle = Math::PiOver2 * 0.5f;
const float rotateSpeed = 40.0f;

// �ő�p
const float maxLookDownAngle = Math::ToRadians(70.0f);
// �ŏ��p
const float minLookDownAngle = Math::ToRadians(00.0f);

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="targetObject">�J��������Ƃ���Q�[���I�u�W�F�N�g�̃|�C���^</param>
ThirdPersonCameraObject::ThirdPersonCameraObject(GameObject* targetObject, TargetSearch* targetSearch)
	:CameraObject(targetObject)
	,mCameraLength(0.0f)
	,mRotateZAngle(-Math::Pi)
	,mLookDownAngle(Math::PiOver2 * 0.35f)
	,mCorrectionValue(17.0f)
	,mIsTargetLocke(false)
	,mIsFinishInterpolation(true)
	,mTargetSearch(targetSearch)
	,mTime(0.0f)
{
	mPlayerDir = targetObject->GetDirection();
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

	Vector3 playerPos = mTargetObject->GetPosition();
	playerPos.z += 100;

	// �L�[����
	const float rotate = 0.6f * deltaTime;
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
	rotatePos.x = mCameraLength * cosf(mLookDownAngle) * cosf(mRotateZAngle);
	rotatePos.y = mCameraLength * cosf(mLookDownAngle) * sinf(mRotateZAngle);
	rotatePos.z = mCameraLength * sinf(mLookDownAngle);

	// ���b�N�I������Ă��Ȃ����
	if (!mIsTargetLocke)
	{
		// �����_�E�J�����ʒu���Z�b�g
		mPosition = rotatePos + playerPos;
		mViewTarget = playerPos;
	}

	if (INPUT_INSTANCE.IsKeyPushdown(KEY_L))
	{
		// ���b�N�I������Ă��Ȃ���΃��b�N�I��
		if (!mIsTargetLocke)
		{
			mIsTargetLocke = true;
			mIsFinishInterpolation = false;

			// �v���C���[�̋߂��̓G������
			mTargetSearch->SearchNearTargetPos(mTargetObject, true);
		}
		else
		{
			mIsTargetLocke = false;
			//mIsFinishInterpolation = false;
			mTime = 0.0f;
		}
	}

	// �J�����̂߂荞�ݖh�~����
	PhysicsWorld* phys = GAMEINSTANCE.GetPhysics();
	CollisionInfo info;

	Line line(playerPos, mPosition);
	
	// ���C�����������Ă�����
	if (phys->SegmentCast(line, info))
	{
		// �Փ˓_�ɃJ�������ړ�
		mPosition = info.mCollisionPoint;
		mPosition -= line.mNormalizeDirection * mCorrectionValue;
	}

	// ���b�N�I������Ă����
	if (mIsTargetLocke)
	{
		TargetLockOn();
	}
	else
	{
		/*if (!mIsFinishInterpolation)
		{
			Vector3 startPos = mViewTarget;
			Vector3 endPos = mTargetObject->GetPosition();
			endPos.z += 100.0f;
			mViewTarget = Vector3::Lerp(startPos, endPos, mTime);
			printf("%f,%f,%f\n", mViewTarget.x, mViewTarget.y, mViewTarget.z);
			mTime += 0.01f;
			if (mTime > 1)
			{
				mIsFinishInterpolation = true;
			}
		}*/
	}
	
	// �J�����s��쐬
	Matrix4 camMat = Matrix4::CreateLookAt(mPosition, mViewTarget, Vector3(0, 0, 1));
	RENDERER->SetViewMatrix(camMat);
	
}

void ThirdPersonCameraObject::TargetLockOn()
{
	// �v���C���[����G�Ɍ������x�N�g���̒P�ʃx�N�g�����擾
	Vector3 enemyPos = mTargetSearch->GetLockeonTargetPos();
	Vector3 playerPos = mTargetObject->GetPosition();
	Vector3 playerToEnemyVec = enemyPos - playerPos;
	playerToEnemyVec.Normalize();

	// �J�����؂�ւ��̕�Ԃ��I�������
	if (mIsFinishInterpolation)
	{
		// �����_�̍X�V
		mViewTarget = enemyPos + playerToEnemyVec;

		// �J�������W�̍X�V
		mPosition = playerPos - playerToEnemyVec * mCameraLength;
		mPosition.z = playerPos.z + mCameraLength;
	}
	else
	{
		// �J�������W�̍X�V
		mPosition = playerPos - playerToEnemyVec * mCameraLength;
		mPosition.z = playerPos.z + mCameraLength;
		playerPos.z += 100.0f;

		// ��Ԃ��Ē����_�̍X�V
		mViewTarget = Vector3::Lerp(playerPos, enemyPos + playerToEnemyVec, mTime);
		mTime += 0.01f;

		if (mTime > 1)
		{
			mIsFinishInterpolation = true;
		}
	}

	// �J�����̂߂荞�ݖh�~����
	PhysicsWorld* phys = GAMEINSTANCE.GetPhysics();
	CollisionInfo info;

	Line line(playerPos, mPosition);

	// ���C�����������Ă�����
	if (phys->SegmentCast(line, info))
	{
		// �Փ˓_�ɃJ�������ړ�
		mPosition = info.mCollisionPoint;
		mPosition -= line.mNormalizeDirection * mCorrectionValue;
	}
}
