#include "StaticBGObject.h"
#include "Game.h"
#include "Renderer.h"
#include "GraphicResourceManager.h"
#include "Mesh.h"
#include "MeshComponent.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="position">位置</param>
/// <param name="gpmeshFileName">メッシュのファイルパス</param>
StaticBGObject::StaticBGObject(const Vector3& position, const char* gpmeshFileName)
	:GameObject(Tag::BackGround)
	,mShaderTag(ShaderTag::SHADOW_MAP)
{
	mPosition = position;
	Mesh* mesh = GraphicResourceManager::GetMesh(gpmeshFileName);
	MeshComponent* mc = new MeshComponent(this,mShaderTag);
	mc->SetMesh(mesh);
}

/// <summary>
/// デストラクタ
/// </summary>
StaticBGObject::~StaticBGObject()
{
}