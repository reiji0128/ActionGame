#pragma once
#include <map>
#include <functional>
#include "DamageType.h"

enum class DamageType;

class DamageNotificator final
{
public:

	/// <summary>
	///  ������
	/// </summary>
	static void Initialize();

	/// <summary>
	/// �I������
	/// </summary>
	static void Finalize();

	/// <summary>
	/// �_���[�W�֐��̒ǉ�
	/// </summary>
	/// <param name="addDamageType">�_���[�W�̎��</param>
	/// <param name="addFunction">�ǉ�����֐�</param>
	static void AddDamageFunction(const DamageType addDamageType, const std::function<int(DamageType)> addFunction);

	/// <summary>
	/// �ʒm����֐�
	/// </summary>
	/// <param name="eventType">�_���[�W�̎��</param>
	static int Norificate(const DamageType eventType);

private:

	/// <summary>
	/// �V���O���g��
	/// </summary>
	DamageNotificator();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~DamageNotificator();

	// ����
	static DamageNotificator* mInstance;

	std::unordered_map<DamageType, std::function<int(DamageType)>> mDamageList;
};
