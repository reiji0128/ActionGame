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
	/// �C�x���g�̒ǉ�
	/// </summary>
	/// <param name="addEventType">�ǉ�����C�x���g�̃^�C�v</param>
	/// <param name="function">�ǉ�����֐�</param>
	void AddEvent(const EventType addEventType ,const std::function<void(int)> addFunction);

	/// <summary>
	/// �ʒm
	/// </summary>
	void Notificate(const EventType eventType);

private:
	std::unordered_map<EventType, std::function<void(int)>> eventList;
};