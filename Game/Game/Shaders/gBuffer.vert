#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoords;

uniform mat4 uWorldTransform; // ���[���h�ϊ��s��
uniform mat4 uViewProj;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	// �ʒu�𓯎����W�n�ɕϊ���
	vec4 pos = vec4(inPosition,1.0);
	// �ʒu�����[���h��Ԃɕϊ�
	pos =  pos *uWorldTransform;
	// ���[���h��Ԃ̈ʒu��ۑ�
	FragPos = pos.xyz;

	Normal = (vec4(inNormal,0.0f) * uWorldTransform).xyz;
	
	// UV���W���t���O�����g�V�F�[�_�ɓn��
	TexCoords   = inTexCoords;

	gl_Position = pos * uViewProj;
}

