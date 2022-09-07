#version 330 core

layout (location = 0) in vec3 aPos;

// �o�͂���e�N�X�`�����W
out vec3 TexCoords;

// �v���W�F�N�V�����s��
uniform mat4 uProjection;

// �r���[�s��
uniform mat4 uView;

uniform mat4 uOffset;


void main()
{
	vec4 pos =  vec4(aPos,1.0f)*uOffset* uView*uProjection ;

	// �[�x�o�b�t�@��z���W��[�x�l�Ƃ��Ď󂯎�邽�߃X�J�C�{�b�N�X�̐[�x�l�����1�ɂ���
	gl_Position = vec4(pos.x,pos.y,pos.w,pos.w);

	// ���W�n���Ⴄ����z���𔽓]
	TexCoords = vec3(aPos.x,aPos.y,-aPos.z);
}