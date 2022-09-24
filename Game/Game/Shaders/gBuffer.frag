#version 330 core
layout (location = 0) out vec3 gPosition;   // G-Buffer�̈ʒu�e�N�X�`���ɏo��
layout (location = 1) out vec3 gNormal;     // G-Buffer�̖@���e�N�X�`���ɏo��
layout (location = 2) out vec4 gAlbedoSpec; // G-Buffer��RGBA�e�N�X�`���ɏo��

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D uDiffuse;  // �I�u�W�F�N�g�̃f�B�t���[�Y�}�b�v
uniform sampler2D uSpecular; // �I�u�W�F�N�g�̃X�y�L�����}�b�v

void main()
{
	// �t���O�����g�̈ʒu��G-Buffer�ɕۑ�
	gPosition = FragPos;
	// �t���O�����g���Ƃ̖@����G-Buffer�ɕۑ�
	gNormal = normalize(Normal);
	// �f�B�t���[�Y�J���[���A���x�h��RGB�ɕۑ�
	gAlbedoSpec.rgb = texture(uDiffuse,TexCoords).rgb;
	// �X�y�L�������x���A���x�h��a�v�f�ɕۑ�
	//gAlbedoSpec.a = texture(uSpecular,TexCoords).r;
	gAlbedoSpec.a = 0.1;
}
