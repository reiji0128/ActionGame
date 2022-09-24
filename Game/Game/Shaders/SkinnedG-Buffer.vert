#version 330 core
layout (location = 0) in vec3  inPosition;
layout (location = 1) in vec3  inNormal;
layout (location = 2) in uvec4 inSkinBones;
layout (location = 3) in vec4  inSkinWeights;
layout (location = 4) in vec2  inTexCoords;

// ���[���h�ϊ��s��
uniform mat4 uWorldTransform; 
// �r���[�ˉe�s��
uniform mat4 uViewProj;
// �s��p���b�g
uniform mat4 uMatrixPalette[196];


out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	// �ʒu�𓯎����W�n�ɕϊ���
	vec4 pos = vec4(inPosition,1.0);

	// �X�L���̈ʒu
	vec4 skinnedPos = (pos * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x;
	     skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	     skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	     skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// ���[���h��Ԃɕϊ�
	skinnedPos = skinnedPos * uWorldTransform;
	// ���[���h��Ԃ̈ʒu��ۑ�
	FragPos = pos.xyz;

	// �X�L���̒��_�̖@��
	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = (skinnedNormal * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x +
		            (skinnedNormal * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y +
		            (skinnedNormal * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z +
		            (skinnedNormal * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	Normal = (skinnedNormal * uWorldTransform).xyz;
	
	// UV���W���t���O�����g�V�F�[�_�ɓn��
	TexCoords   = inTexCoords;

	gl_Position = skinnedPos * uViewProj;
}

