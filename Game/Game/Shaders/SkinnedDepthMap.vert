#version 330 core

// ���[���h��ԍs��
uniform mat4 uWorldTransform;

// �s��p���b�g
uniform mat4 uMatrixPalette[196];
uniform mat4 uLightSpaceMat;

// Attribute 0 �ʒu    Attribute 1 �@��  Attribute 2 �{�[�� 
// Attribute 3 �d��    Attribute 4 �e�N�X�`�����W

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in uvec4 inSkinBones;
layout (location = 3) in vec4 inSkinWeights;
layout (location = 4) in vec2 inTexCoord;

void main()
{
	// �������W�n�ɕϊ�
	vec4 pos = vec4(inPosition,1.0);

	//�X�L���̈ʒu
	vec4 skinnedPos = (pos * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	//���C�g��Ԃ̃t���O�����g�ʒu���o��
	gl_Position = skinnedPos * uWorldTransform * uLightSpaceMat;
}
