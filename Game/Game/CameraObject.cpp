#include "CameraObject.h"
#include "Game.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="targetActor">�J��������Ƃ���Q�[���I�u�W�F�N�g�̃|�C���^</param>
CameraObject::CameraObject(GameObject* targetActor)
	:GameObject(Tag::Camera)
	,mTargetActor(targetActor)
{
	mPosition = Vector3(0, 100, 100);

	// �f�t�H���g�J�����l�Z�b�g
	mViewMatrix = Matrix4::CreateLookAt(Vector3(-100, 0, 100),
		                                Vector3(0, 0, 0),
		                                Vector3(0, 0, 1));

	// �J�������A�N�e�B�u�ɃZ�b�g
	SetActive();
}

CameraObject::~CameraObject()
{
	// �Q�[���V�X�e�����ɔ�A�N�e�B�u��ʒm
	GAMEINSTANCE.SetInActiveCameraObject(this);
}

void CameraObject::Update(float deltaTime)
{
	UpdateGameObject(deltaTime);

	// �J���������x�N�g��,�J�����s��쐬
	mViewVector = mViewTarget - mPosition;
	mViewMatrix = Matrix4::CreateLookAt(mPosition,
		                                mViewTarget,
		                                Vector3(0, 0, 1));
}

void CameraObject::SetActive()
{
	// �Q�[���V�X�e�����ɃA�N�e�B�u��ʒm
	GAMEINSTANCE.SetCameraObject(this);
}

void CameraObject::Init(const Vector3& cameraPos, const Vector3& targetPos, const Vector3& upVec)
{
	mPosition = cameraPos;
	mViewTarget = targetPos;
	mViewVector = targetPos - cameraPos;

	mViewMatrix = Matrix4::CreateLookAt(cameraPos,
		                                targetPos,
		                                upVec);
}
