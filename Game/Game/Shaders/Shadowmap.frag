#version 330 core

layout (location = 0) out vec4 FragBuffer;   // HDR�o�b�t�@
layout (location = 1) out vec4 BrightBuffer; // ���P�x�o�b�t�@

// �f�B���N�V���i�����C�g�\����
struct DirectionalLight
{
	// ���C�g�̕���
	vec3 mDirection;
	// �f�B�t�[�Y�F
	vec3 mDiffuseColor;
	// �X�y�L�����[�F
	vec3 mSpecColor;
};

// �e�N�X�`�����W
in  vec2 fragTexCoord;
// �@��
in vec3 fragNormal;
// ���_�ʒu
in vec3 fragWorldPos;
// ���C�g��Ԃł̃t���O�����g���W
in vec4 fragPosLightSpace; 

// ���C�e�B���O�p�ϐ� //
// �J�����ʒu
uniform vec3 uCameraPos;
// �|���S���\�ʂ̃X�y�L�����[���x
uniform float uSpecPower;
// �f�B���N�V���i�����C�g
uniform DirectionalLight uDirLight;
// �A���r�G���g���C�g�F
uniform vec3 uAmbientLight;

uniform sampler2D uTexture;
uniform sampler2D depthMap;

// �o�̓J���[
out vec4 outColor;

float ShadowCaluculation(vec4 fragPosLightSpace)
{
	// �p�[�X�y�N�e�B�u���Z
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// [0,1]�͈̔͂ɕϊ�
	projCoords      = projCoords * 0.5 + 0.5;
	// �V���h�E�}�b�v��胉�C�g�ɍł��߂��t���O�����g�̐[�x�l�𓾂�
	float closestDepth = texture(depthMap,projCoords.xy).r;
	// ���ݕ`�悵�悤�Ƃ��Ă���t���O�����g�̐[�x�l
	float currentDepth = projCoords.z;
	// �V���h�E����(1.0:�V���h�E  0.0:�V���h�E�O)
	float bias = max(0.0005 * (1.0 - dot(normalize(fragNormal),uDirLight.mDirection)),0.00005);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;
	return shadow;
}

void main()
{
	// �|���S���\�ʂ̖@���i�t���O�����g�V�F�[�_�[��ŕ�Ԃ���Ă���j
	vec3 N = normalize(fragNormal);
	// �|���S���\�ʂ��烉�C�g�����ւ̃x�N�g��
	vec3 L = normalize(-uDirLight.mDirection);
	// �|���S���\�ʂ���J��������
	vec3 V = normalize(uCameraPos - fragWorldPos);
	// -L �x�N�g���� �@�� N �ɑ΂��Ĕ��˂����x�N�g��R�����߂�
	vec3 R = normalize(reflect(-L, N));

	// �t�H�����ˌv�Z
	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);

	vec3 Diffuse;
	vec3 Specular;

	Diffuse = uDirLight.mDiffuseColor * max(NdotL,0.0f);
	Specular = uDirLight.mSpecColor * pow(max(0.0, dot(R, V)), uSpecPower);

	float shadow = ShadowCaluculation(fragPosLightSpace);

	vec3 texColor = texture(uTexture,fragTexCoord).rgb;
	vec3 diffuseColor = Diffuse * texColor;
	vec3 ambientColor = uAmbientLight* texColor;

	vec3 result = ambientColor + (1.0 - shadow) * (diffuseColor + Specular);
	//outColor = vec4(result,1.0f);

	float brightness = dot(result,vec3(0.2126,0.7152,0.0722));
	if(brightness > 0.8)
	{
		BrightBuffer = vec4(result,0.0f);
	}
	else
	{
		BrightBuffer = vec4(0.0f,0.0f,0.0f,0.0f);
	}
	FragBuffer = vec4(result,1.0);
}