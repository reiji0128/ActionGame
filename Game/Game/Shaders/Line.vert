#version 330

// �r���[�ˉe�p��Uniform�ϐ�
uniform mat4 uViewProj;

// Attribute 0 �ʒu
layout(location = 0) in vec3 inPosition;

void main()
{
	// ���W�𓯎����W�ɕϊ�����
	vec4 pos = vec4(inPosition, 1.0);

	// ��Ԃɕϊ�
	gl_Position = pos * uViewProj;

}
