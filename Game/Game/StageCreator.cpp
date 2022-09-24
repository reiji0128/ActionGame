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
	// プレイヤーの生成
	mPlayer = new PlayerObject(Vector3(890, -50, 110),                   // 座標
		                       1.0f,                                     // スケール
		                       "Assets/Player/Great_Sword_Idle.gpmesh",  // gpMeshのファイルパス
		                       "Assets/Player/Great_Sword_Idle.gpskel"); // gpSkelのファイルパス

	// 武器の生成
	mSword = new SwordObject(Vector3(90, 20, 140),                     // オフセット位置
		                     Vector3(90, 60, -30),                     // オフセット角度(radian)
		                     "Assets/Sword/SK_Sword.gpmesh",           // gpMeshのファイルパス
		                     mPlayer->GetSkeltalMeshComp(),            // アタッチ先のSkeltalMeshCompクラスのポインタ
		                     "LeftHandIndex4");                        // アタッチ先のボーン名

	new EnemyObject(Vector3(2941, -949, 112),
		             0.5f,
		             "Assets/Dragon/DRAGON.gpmesh",
		             "Assets/Dragon/DRAGON.gpskel");

	// カメラの生成
	mCamera= new ThirdPersonCameraObject(mPlayer);
	mCamera->SetCameraLength(500.0f);

	new PointLight(Vector3(0, 0, 0));

	// バックグラウンドの生成
	new StaticBGObject(Vector3(1430, -1125, 110), "Assets/BackGround/FloorMesh.gpmesh");
	new StaticBGObject(Vector3(1885, -980, 100), "Assets/BackGround/WallMesh.gpmesh");

	// バックグラウンドの当たり判定の生成
	new BGCollisionSetter("Assets/BackGround/BackGroundCollision.json");

	new SkyBoxObject();
}
