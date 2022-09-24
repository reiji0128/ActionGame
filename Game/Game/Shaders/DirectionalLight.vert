#version 330

// Attribute 0 ���W, Attribute 1 uv���W
layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inTexCoord;

// uv���W�o�͕ϐ�
out vec2 fragTexCoord;

void main()
{
	// �e�N�X�`�����W���t���O�����g�V�F�[�_�[�ɓn��
	fragTexCoord = inTexCoord;
	gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
}