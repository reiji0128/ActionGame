#pragma once
#include <map>
#include <functional>
#include "DamageType.h"

enum class DamageType;

class DamageNotificator final
{
public:

	/// <summary>
	///  初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 終了処理
	/// </summary>
	static void Finalize();

	/// <summary>
	/// ダメージ関数の追加
	/// </summary>
	/// <param name="addDamageType">ダメージの種類</param>
	/// <param name="addFunction">追加する関数</param>
	static void AddDamageFunction(const DamageType addDamageType, const std::function<int(DamageType)> addFunction);

	/// <summary>
	/// 通知する関数
	/// </summary>
	/// <param name="eventType">ダメージの種類</param>
	static int Norificate(const DamageType eventType);

private:

	/// <summary>
	/// シングルトン
	/// </summary>
	DamageNotificator();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DamageNotificator();

	// 実体
	static DamageNotificator* mInstance;

	std::unordered_map<DamageType, std::function<int(DamageType)>> mDamageList;
};
