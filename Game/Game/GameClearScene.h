#pragma once
#include "Game.h"
#include "SceneBase.h"
#include "Renderer.h"

class GameClearScene : public SceneBase
{
public:
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	GameClearScene();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~GameClearScene();

	/// <summary>
	/// �V�[���̕`��
	/// </summary>
	/// <returns></returns>
	SceneBase* Update();

	void Draw();

private:
	// �r�b�g�}�b�v�e�L�X�g�̃|�C���^
	class BitmapText* mFont;

	// �e�N�X�`���N���X�̃|�C���^
	class Texture* mGameClearTex;
};