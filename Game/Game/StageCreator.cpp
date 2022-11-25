#include "StageCreator.h"
#include "PlayerObject.h"
#include "SwordObject.h"
#include "EnemyObject.h"
#include "ThirdPersonCameraObject.h"
#include "StaticBGObject.h"
#include "PointLight.h"
#include "BGCollisionSetter.h"
#include "SkyBoxObject.h"
#include "PointLight.h"
#include "EventNotificator.h"

StageCreator::StageCreator()
{
	CreateStage();
}

StageCreator::~StageCreator()
{
}

//IHitPointGetter* StageCreator::GetPlayerHPGetter() const
//{
//	//return static_cast<IHitPointGetter*>(mPlayer);
//}
//
//IHitPointGetter* StageCreator::GetEnemyHPGetter() const
//{
//	//return static_cast<IHitPointGetter*>(mEnemy);
//}

void StageCreator::CreateStage()
{
	// バックグラウンドの生成
	new StaticBGObject(Vector3(1430, -1125, 110), "Assets/BackGround/FloorMesh.gpmesh");
	new StaticBGObject(Vector3(1885, -980, 100), "Assets/BackGround/WallMesh.gpmesh");

	// ライトの生成
	new PointLight(Vector3(407.0f, -2600.0f, 500.0f) ,Vector3(1,0,0));
	new PointLight(Vector3(1229.0f, -2600.0f, 500.0f), Vector3(1, 0, 0));
	new PointLight(Vector3(2000.0f, -2600.0f, 500.0f), Vector3(1, 0, 0));
	new PointLight(Vector3(2800.0f, -2600.0f, 500.0f), Vector3(1, 0, 0));

	new PointLight(Vector3(407.0f,  900.0f, 500.0f), Vector3(-1, 0, 0));
	new PointLight(Vector3(1229.0f, 900.0f, 500.0f), Vector3(-1, 0, 0));
	new PointLight(Vector3(2000.0f, 900.0f, 500.0f), Vector3(-1, 0, 0));
	new PointLight(Vector3(2800.0f, 900.0f, 500.0f), Vector3(-1, 0, 0));

	new PointLight(Vector3(-319.0f, -2152.0f, 500.0f), Vector3(0, -1, 0));
	new PointLight(Vector3(-319.0f, -1552.0f, 500.0f), Vector3(0, -1, 0));
	new PointLight(Vector3(-319.0f, -952.0f, 500.0f), Vector3(0, -1, 0));
	new PointLight(Vector3(-319.0f, -352.0f, 500.0f), Vector3(0, -1, 0));
	new PointLight(Vector3(-319.0f, 352.0f, 500.0f), Vector3(0, -1, 0));

	new PointLight(Vector3(3770.0f, -2152.0f, 500.0f), Vector3(0, 1, 0));
	new PointLight(Vector3(3770.0f, -1552.0f, 500.0f), Vector3(0, 1, 0));
	new PointLight(Vector3(3770.0f, -952.0f, 500.0f), Vector3(0, 1, 0));
	new PointLight(Vector3(3770.0f, -352.0f, 500.0f), Vector3(0, 1, 0));
	new PointLight(Vector3(3770.0f, 352.0f, 500.0f), Vector3(0, 1, 0));

	// バックグラウンドの当たり判定の生成
	new BGCollisionSetter("Assets/BackGround/BackGroundCollision.json");

	new SkyBoxObject();
}
