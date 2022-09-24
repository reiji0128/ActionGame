#version 330 core

layout(location = 0) out vec4 FragBuffer;          // HDR�o�b�t�@�ɏo��
layout(location = 1) out vec4 BrightBuffer;        // ���P�x�o�b�t�@�ɏo��

// �f�B���N�V���i�����C�g�ݒ�
struct DirectionalLight
{
	vec3 direction;
	vec3 ambientColor;
	vec3 color;
	vec3 specular;
	float intensity;
};

// �f�B���N�V���i�����C�g
uniform DirectionalLight uLight;

// G-Buffer���[���h�ʒu
uniform sampler2D gPosition;
// G-Buffer���[���h�@��
uniform sampler2D gNormal;
// G-Buffer�A���x�h & �X�y�L����
uniform sampler2D gAlbedoSpec;

uniform sampler2D uDepthMap;
// ���_
uniform vec3 uViewPos;

// �P�x
uniform float luminance;

// ���C�g��ԍs��
uniform mat4 uLightSpaceMatrix;

// ����uv���W
in vec2 fragTexCoord;

float ShadowCaluculation(vec4 fragPosLightSpace,vec3 Normal,vec3 LightDir)
{
	// �p�[�X�y�N�e�B�u���Z
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	// [0,1]�͈̔͂ɕϊ�
	projCoords = projCoords * 0.5 + 0.5;
	// �V���h�E�}�b�v��胉�C�g�ɍł��߂��t���O�����g�̐[�x�l�𓾂�
	float closestDepth = texture(uDepthMap,projCoords.xy).r;
	// �`�悵�悤�Ƃ��Ă���t���O�����g�̐[�x�l
	float currentDepth = projCoords.z;
	// �V���h�E����(1.0 : �V���h�E  0.0 : �V���h�E�O)
	float bias = max(0.0005 * (1.0 - dot(normalize(Normal),LightDir)),0.00005);
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	return shadow;
}

void main()
{
	// gBuffer���WorldPos,Normal,�A���x�h���擾
	vec3 Position   = texture(gPosition,fragTexCoord).xyz;
	vec3 Normal     = texture(gNormal,fragTexCoord).xyz;
	vec4 albedoSpec = texture(gAlbedoSpec,fragTexCoord);
	vec3 Albedo     = albedoSpec.rgb;
	float Spec_p    = albedoSpec.a;

	vec3 ambient    = uLight.ambientColor * Albedo;
	vec3 LightDir   = normalize(-uLight.direction);
	Normal          = normalize(Normal);
	float diff      = max(dot(Normal,LightDir),0.0f);

	// �f�B�t���[�Y
	vec3 diffuse    = uLight.color * uLight.intensity * Albedo * diff;

	// �X�y�L�����[
	vec3 reflectDir = reflect(LightDir,Normal);
	vec3 viewDir    = normalize(uViewPos - Position);
	vec3 halfVec    = normalize(LightDir + viewDir);
	float spec      = pow(max(dot(Normal,halfVec),0.0),32);
	vec3 specular   = uLight.specular * uLight.intensity * spec * Spec_p;

	// ���C�g��ԍs��̍쐬
	vec4 fragPosLightSpace;
	fragPosLightSpace = vec4(Position,1.0) * uLightSpaceMatrix;

	// �V���h�E�̌v�Z
	float shadow = ShadowCaluculation(fragPosLightSpace,Normal,LightDir);

	vec3 result = ambient + (1.0 - shadow) * (diffuse + specular) * luminance;

	// �s�N�Z���̋P�x�����߂�
	float brightness = dot(result,vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 0.8)
	{
		BrightBuffer = vec4(result, 0.0f);
	}
	else
	{
		BrightBuffer = vec4(0.0f,0.0f,0.0f,0.0f);
	}
	FragBuffer = vec4(result,1.0);
}