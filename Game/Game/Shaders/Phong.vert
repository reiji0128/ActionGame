#version 330

// ���[���h�ϊ������p��Uniform�ϐ�
uniform mat4 uWorldTransform;
// �r���[�ˉe�ϊ��p��Uniform�ϐ�
uniform mat4 uViewProj;

// Attribute 0 �ʒu ,Attribute 1 �@�� ,Attribute 2 UV���W.
layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// �C�ӂ̒��_�o��
out vec2 fragTexCoord;
// �@�� (in world space)
out vec3 fragNormal;
// �ʒu (in world space)
out vec3 fragWorldPos;

void main()
{
	// �ʒu�𓯎����W�n�ɕϊ�����
	vec4 pos = vec4(inPosition, 1.0);
	// �ʒu�����[���h��Ԃɕϊ�����
	pos = pos * uWorldTransform;
	// ���[���h��Ԃ̈ʒu��ۑ�
	fragWorldPos = pos.xyz;
	// �N���b�v��Ԃɕϊ�����
	gl_Position = pos * uViewProj;

	// �@�������[���h��Ԃɕϊ� (w = 0)
	fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

	// UV���W���t���O�����g�V�F�[�_�[�ɓn��
	fragTexCoord = inTexCoord;
}
