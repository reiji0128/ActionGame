#pragma once

#include "GameObject.h"
#include "Math.h"
#include "ShaderTag.h"

class StaticBGObject : public GameObject
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="position">位置</param>
	/// <param name="gpmeshFileName">メッシュのファイルパス</param>
	StaticBGObject(const Vector3& position, const char* gpmeshFileName);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~StaticBGObject();

private:
	ShaderTag mShaderTag;
};
