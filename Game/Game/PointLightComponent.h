#pragma once
#include "Component.h"
#include "Math.h"

// �O���錾
class GameObject;
class Shader;
class Mesh;


class PointLightComponent : public Component
{
public:
	PointLightComponent(GameObject* owner,const Vector3& offset, const Vector3& color);

	~PointLightComponent();

	void Draw(Shader* shader, Mesh* sphereMesh);

	/// <summary>
	/// ���C�g�̌������a�̌v�Z
	/// </summary>
	/// <param name="constant">�萔</param>
	/// <param name="linear">���`</param>
	/// <param name="quadratic">2�捀</param>
	void CalcAttenuationLightRadius(const float constant, const float linear, const float quadratic);

private:

	Vector3 mOffset;

	Vector3 mColor;

	float mRadius;

	bool mIsVisible;
};
