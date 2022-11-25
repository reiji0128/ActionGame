#include "DamageNotificator.h"

DamageNotificator* DamageNotificator::mInstance = nullptr;

DamageNotificator::DamageNotificator()
{
	mInstance = nullptr;
}

DamageNotificator::~DamageNotificator()
{
}

void DamageNotificator::Initialize()
{
	if (!mInstance)
	{
		mInstance = new DamageNotificator;
	}
}

void DamageNotificator::Finalize()
{
	if (mInstance)
	{
		delete mInstance;
		mInstance = nullptr;
	}
}

void DamageNotificator::AddDamageFunction(const DamageType addDamageType, const std::function<int(DamageType)> addFunction)
{
	mInstance->mDamageList.insert(std::pair(addDamageType, addFunction));
}

int DamageNotificator::Norificate(const DamageType damageType)
{

	auto range = mInstance->mDamageList.equal_range(damageType);

	for (auto iter = range.first; iter != range.second; iter++)
	{
		auto pair = *iter;
		return pair.second(damageType);
	}
}

