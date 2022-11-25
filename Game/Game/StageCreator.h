#pragma once

// 前方宣言

class IHitPointGetter;

class StageCreator
{
public:
	StageCreator();

	~StageCreator();

// ゲッター //
	//IHitPointGetter* GetPlayerHPGetter() const;

	//IHitPointGetter* GetEnemyHPGetter() const;

private:
	void CreateStage();
};
