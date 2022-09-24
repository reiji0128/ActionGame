#include "PointLight.h"
#include "GraphicResourceManager.h"
#include "Mesh.h"
#include "MeshComponent.h"

PointLight::PointLight(const Vector3& pos)
	:GameObject(Tag::PointLight)
	,mShaderTag(ShaderTag::HDR)
{
	mPosition = pos;
	Mesh* mesh = GraphicResourceManager::LoadMesh("Assets/Sphere/Sphere.gpmesh");
	MeshComponent* mc = new MeshComponent(this, mShaderTag);
	mc->SetMesh(mesh);
}

PointLight::~PointLight()
{
}
