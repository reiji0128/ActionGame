#pragma once
#include <map>
#include <functional>
#include "EventType.h"

enum class EventType;

class EventNotificator final
{
public:
	EventNotificator();

	~EventNotificator();

	/// <summary>
	/// イベントの追加
	/// </summary>
	/// <param name="addEventType">追加するイベントのタイプ</param>
	/// <param name="function">追加する関数</param>
	void AddEvent(const EventType addEventType ,const std::function<void(int)> addFunction);

	/// <summary>
	/// 通知
	/// </summary>
	void Notificate(const EventType eventType);

private:
	std::unordered_map<EventType, std::function<void(int)>> eventList;
};