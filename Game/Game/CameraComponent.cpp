#include "CameraComponent.h"
#include "Actor.h"
#include "Renderer.h"
#include "Game.h"
#include "AudioManager.h"

CameraComponent::CameraComponent(Actor* owner, int updateOrder)
	:Component(owner, updateOrder)
{
}

void CameraComponent::SetViewMatrix(const Matrix4& view)
{
	// �r���[�s��������_���[�ƃI�[�f�B�I�V�X�e���ɓn��
	GAMEINSTANCE.GetRenderer()->SetViewMatrix(view);
}
