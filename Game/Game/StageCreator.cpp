#include "StageCreator.h"
#include "PlayerObject.h"
#include "SwordObject.h"
#include "EnemyObject.h"
#include "ThirdPersonCameraObject.h"
#include "StaticBGObject.h"
#include "BGCollisionSetter.h"
#include "SkyBoxObject.h"
#include "PointLight.h"

StageCreator::StageCreator()
{
	CreateStage();
}

StageCreator::~StageCreator()
{
	delete mPlayer;
	delete mSword;
	delete mCamera;
}

void StageCreator::CreateStage()
{
	// �v���C���[�̐���
	mPlayer = new PlayerObject(Vector3(890, -50, 110),                   // ���W
		                       1.0f,                                     // �X�P�[��
		                       "Assets/Player/Great_Sword_Idle.gpmesh",  // gpMesh�̃t�@�C���p�X
		                       "Assets/Player/Great_Sword_Idle.gpskel"); // gpSkel�̃t�@�C���p�X

	// ����̐���
	mSword = new SwordObject(Vector3(90, 20, 140),                     // �I�t�Z�b�g�ʒu
		                     Vector3(90, 60, -30),                     // �I�t�Z�b�g�p�x(radian)
		                     "Assets/Sword/SK_Sword.gpmesh",           // gpMesh�̃t�@�C���p�X
		                     mPlayer->GetSkeltalMeshComp(),            // �A�^�b�`���SkeltalMeshComp�N���X�̃|�C���^
		                     "LeftHandIndex4");                        // �A�^�b�`��̃{�[����

	new EnemyObject(Vector3(2941, -949, 112),
		             0.5f,
		             "Assets/Dragon/DRAGON.gpmesh",
		             "Assets/Dragon/DRAGON.gpskel");

	// �J�����̐���
	mCamera= new ThirdPersonCameraObject(mPlayer);
	mCamera->SetCameraLength(500.0f);

	new PointLight(Vector3(0, 0, 0));

	// �o�b�N�O���E���h�̐���
	new StaticBGObject(Vector3(1430, -1125, 110), "Assets/BackGround/FloorMesh.gpmesh");
	new StaticBGObject(Vector3(1885, -980, 100), "Assets/BackGround/WallMesh.gpmesh");

	// �o�b�N�O���E���h�̓����蔻��̐���
	new BGCollisionSetter("Assets/BackGround/BackGroundCollision.json");

	new SkyBoxObject();
}
