#include "SkyBoxObject.h"
#include "CubeMapComponent.h"
#include "Game.h"
#include "Renderer.h"

SkyBoxObject::SkyBoxObject()
	:GameObject(Tag::SkyBox)
{
	// �L���[�u�}�b�v�R���|�[�l���g�𐶐�
	mCubeMapComp = new CubeMapComponent(this);

	// �����_���[�ɃL���[�u�}�b�v�R���|�[�l���g�̃|�C���^��n��
	RENDERER->SetActiveSkyBox(mCubeMapComp);

	// �g�p����e�N�X�`���𐶐�
	mCubeMapComp->CreateTexture("Assets/Skybox/");
}

SkyBoxObject::~SkyBoxObject()
{
}
