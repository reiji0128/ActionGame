#version 330

// ���_�V�F�[�_�[����e�N�X�`�����W�����
in vec2 fragTexCoord;

// �J���[�o�b�t�@�ւ̏o�͐F
out vec4 outColor;

// �e�N�X�`���T���v�����O�Ɏg�p
uniform sampler2D uTexture;
uniform float alpha;

void main()
{
	// �e�N�X�`������̃T���v���J���[
	vec4 color = texture(uTexture,fragTexCoord);
	color.a = color.a * alpha;
	outColor = color;
}