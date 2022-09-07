#version 330 core

layout (location = 0) in vec3 inPosition;     // ���_�ʒu
layout (location = 1) in vec3 inNormal;       // �@��
layout (location = 2) in vec2 inTexCoords;    // �e�N�X�`�����W

uniform mat4 uWorldTransform;                 // ���[���h�ϊ��s��
uniform mat4 uView;                           // �r���[�s��
uniform mat4 uProjection;                     // �v���W�F�N�V�����s��
uniform mat4 uLightSpaceMatrix;               // ���C�g��ԍs��

out vec3 fragNormal;                          // �t���O�����g�ւ̖@���o��
out vec3 fragWorldPos;                        // �t���O�����g�ւ̍��W�o��
out vec2 fragTexCoord;                        // �e�N�X�`�����W
out vec4 fragPosLightSpace;                   // ���C�g��Ԃł̃t���O�����g���W

void main()
{
	// �ʒu�𓯎����W�n�ɕϊ���
	vec4 pos = vec4(inPosition,1.0);
	// �ʒu�����[���h��Ԃɕϊ�
	pos =  pos *uWorldTransform;
	// ���[���h��Ԃ̈ʒu��ۑ�
	fragWorldPos = pos.xyz;
	// �N���b�v��Ԃɕϊ�
	gl_Position = pos * uView * uProjection ;

	fragNormal = (vec4(inNormal,0.0f) * uWorldTransform).xyz;
	fragPosLightSpace = vec4(fragWorldPos,1.0) * uLightSpaceMatrix;
	
	// UV���W���t���O�����g�V�F�[�_�ɓn��
	fragTexCoord   = inTexCoords;
}