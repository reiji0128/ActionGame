#version 330 core

layout(location = 0) out vec4 FragBuffer;          // HDR�o�b�t�@�ɏo��
layout(location = 1) out vec4 BrightBuffer;        // ���P�x�o�b�t�@�ɏo��

// �|�C���g���C�g
struct PointLight
{
	// �����ʒu
	vec3 position;
	// ���C�g�A���r�G���g
	vec3 ambient;
	// ���C�g�f�B�t���[�Y
	vec3 diffuse;
	// ���C�g�X�y�L�����[
	vec3 specular;

	// �萔
	float constant;
	// ���`��
	float linear;
	// 2�捀
	float quadratic;
};

uniform PointLight uLight;

// G-Buffer���[���h�ʒu
uniform sampler2D gPosition;
// G-Buffer���[���h�@��
uniform sampler2D gNormal;
// G-Buffer�A���x�h & �X�y�L�����[���x
uniform sampler2D gAlbedoSpec;
// ���_
uniform vec3      uViewPos;

// �P�x
uniform float luminance;

// uv���W
in vec2 TexCoords;
// �t���O�����g�̏o��
out vec4 FragColor;

void main()
{
	// G-Buffer����WorldPos,Normal,�A���x�h���擾
	vec3  Position    = texture(gPosition,TexCoords).xyz;
	vec3  Normal      = texture(gNormal,TexCoords).xyz;
	vec4  AlbedoSpec  = texture(gAlbedoSpec,TexCoords);
	vec3  Albedo      = AlbedoSpec.rgb;
	float Spec_p      = AlbedoSpec.a;

	// ���C�g�Ƃ̋���
	float l_distance = length(uLight.position - Position);
	float attenuation = max(1.0f - l_distance / 100.0f, 0.0f);

	// �f�B�t���[�Y
	vec3  norm     = normalize(Normal);
	vec3  lightDir = normalize(uLight.position - Position);
	float diff     = max(dot(norm,lightDir),0.0);
	vec3  diffuse  = uLight.diffuse * diff * Albedo;

	// �X�y�L�����[
	vec3 viewDir   = normalize(uViewPos - Position);
	vec3 halfDir   = normalize(lightDir + viewDir);
	float spec     = pow(max(dot(norm,halfDir),0.0),32);
	vec3 specular  = uLight.specular * spec * Spec_p;

	// �A���r�G���g
	vec3 ambient   = uLight.ambient * Albedo;

	ambient       *= attenuation;
	diffuse       *= attenuation;
	specular      *= attenuation;

	vec3 result   = (ambient + diffuse + specular) * luminance;
	
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