#version 330 core

// ���_�V�F�[�_�[����uv���W�����
in vec2 fragTexCoord;

// �J���[�o�b�t�@�[�ւ̏o��
out vec4 outColor;

// �o�b�t�@
uniform sampler2D uBuffer;

// �I�o
uniform float exposure;

void main()
{
	vec3 hdrColor = texture(uBuffer,fragTexCoord).rgb;
	outColor = vec4(hdrColor,1.0);
}