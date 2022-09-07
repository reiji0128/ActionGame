#include "CubeMapComponent.h"
#include "Game.h"
#include "Renderer.h"
#include "VertexArray.h"
#include "GameObject.h"
#include "Texture.h"
#include "Shader.h"

/// <summary>
/// �R���X�g���N�^
/// </summary>
/// <param name="owner">�Q�[���I�u�W�F�N�g�̃|�C���^</param>
CubeMapComponent::CubeMapComponent(GameObject* owner)
	:Component(owner)
	,mVisible(true)
{
}

/// <summary>
/// �f�X�g���N�^
/// </summary>
CubeMapComponent::~CubeMapComponent()
{
}

/// <summary>
/// �e�N�X�`���̍쐬
/// </summary>
/// <param name="fileName">�e�N�X�`���̃t�@�C���p�X</param>
void CubeMapComponent::CreateTexture(const std::string& fileName)
{
	mTexture = new Texture();
	mTexture->LoadCubeMap(fileName);
}

/// <summary>
/// �`�揈��
/// </summary>
/// <param name="shader">�g�p����V�F�[�_�[</param>
void CubeMapComponent::Draw(Shader* shader)
{
	if (mVisible)
	{
		glDepthFunc(GL_LEQUAL);
		RENDERER->GetCubeMapVerts()->SetActive();

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, mTexture->GetTextureID());

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
		glDepthFunc(GL_LESS);
	}
}
