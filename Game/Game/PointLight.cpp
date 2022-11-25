#include "PointLight.h"
#include "GraphicResourceManager.h"
#include "Mesh.h"
#include "MeshComponent.h"
#include "PointLightComponent.h"

PointLight::PointLight(const Vector3& pos,const Vector3& dir)
	:GameObject(Tag::PointLight)
	,mShaderTag(ShaderTag::SHADOW_MAP)
{
	SetScale(2.0f);
	mPosition = pos;
	mDirection = dir;

	Mesh* mesh = GraphicResourceManager::LoadMesh("Assets/Lantern/Lantern.gpmesh");
	MeshComponent* mc = new MeshComponent(this, mShaderTag);
	mc->SetMesh(mesh);

	new PointLightComponent(this, Vector3(0, 0, -100), Vector3(1.0f, 0.55f, 0.0f),10.0f);
}

PointLight::~PointLight()
{
}
