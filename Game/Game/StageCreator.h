#pragma once

class PlayerObject;
class SwordObject;
class ThirdPersonCameraObject;

class StageCreator
{
public:
	StageCreator();

	~StageCreator();

private:
	void CreateStage();

	PlayerObject* mPlayer;
	SwordObject* mSword;
	ThirdPersonCameraObject* mCamera;
};
