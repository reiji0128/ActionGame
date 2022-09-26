#pragma once

class EnemyObject;

class EnemyStateThink
{
public:
	EnemyStateThink();

	~EnemyStateThink();

	void ThinkNextState(EnemyObject* owner, float deltaTime);
};