#pragma once
#include "Component.h"
#include <SDL.h>

// 前方宣言
class GameObject;
class Shader;
class Texture;


class SpriteComponent :public Component
{
public:
	SpriteComponent(GameObject* owner, int drawOrder = 100);
	~SpriteComponent();

	virtual void Draw(Shader* shader);
	virtual void SetTexture(Texture* texture);

	int GetDrawOrder() const { return mDrawOrder; }
	int GetTexHeight() const { return mTexHeight; }
	int GetTexWidth() const { return mTexWidth; }
private:
	Texture* mTexture;
	int mDrawOrder;
	int mTexHeight;
	int mTexWidth;
};