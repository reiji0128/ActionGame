#pragma once

// �K�p����V�F�[�_�[�̃^�O
enum class ShaderTag
{					    
	SPRITE,              // �X�v���C�g
	TILEMAP,             // �^�C���}�b�v
	PHONG,               // �t�H�O
	DEPTH_MAP,           // �f�v�X�}�b�v(���b�V���p)
	SKINNED_DEPTH_MAP,   // �f�v�X�}�b�v(�X�P���^�����b�V���p)
	SHADOW_MAP,          // �V���h�E�}�b�v(���b�V���p)
	SKINNED_SHADOW_MAP,  // �V���h�E�}�b�v(�X�P���^�����b�V���p)
	HDR,                 // HDR
	DOWNSAMPLING,        // �_�E���T���v�����O
	GAUSS_BLUR,          // �K�E�X�u���[
	HDR_BLOOM_BLEND,     // HDR�u���[���u�����h
	NORMAL_MAP,		     // �@���}�b�v
	SCREEN_BUFFER,       // �X�N���[���o�b�t�@
	SKYBOX,              // �X�J�C�{�b�N�X
	G_BUFFER,            // G-Buffer
	SKINNED_G_BUFFER,    // �X�L���pG-Buffer
	POINT_LIGHT,         // �|�C���g���C�g
	DIRECTIONAL_LIGHT,   // �f�B���N�V���i�����C�g
	HIGH_LIGHT,          // �n�C���C�g
	EMISSIVE,            // �G�~�b�V�u

	NONE,                // �K�p����V�F�[�_�[�Ȃ�
};