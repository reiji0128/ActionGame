#pragma once

// �O���錾

class IHitPointGetter;

class StageCreator
{
public:
	StageCreator();

	~StageCreator();

// �Q�b�^�[ //
	//IHitPointGetter* GetPlayerHPGetter() const;

	//IHitPointGetter* GetEnemyHPGetter() const;

private:
	void CreateStage();
};
