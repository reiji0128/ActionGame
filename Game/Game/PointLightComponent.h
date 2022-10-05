#pragma once
#include "Component.h"
#include "Math.h"

// 前方宣言
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
	/// ライトの減衰半径の計算
	/// </summary>
	/// <param name="constant">定数</param>
	/// <param name="linear">線形</param>
	/// <param name="quadratic">2乗項</param>
	void CalcAttenuationLightRadius(const float constant, const float linear, const float quadratic);

private:

	Vector3 mOffset;

	Vector3 mColor;

	float mRadius;

	bool mIsVisible;
};
