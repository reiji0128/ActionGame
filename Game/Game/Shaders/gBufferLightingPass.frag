#version 330 core

// uv���W
in vec2 fragTexCoord;

// �o�͐F
out vec4 FragColor;

// ���C�g�\����
struct Light
{
	vec3 Position;
	vec3 Color;
};

// GBuffer �ʒu
uniform sampler2D gPosition;
// GBuffer �@��
uniform sampler2D gNormal;
// GBuffer �A���x�h + �X�y�L����
uniform sampler2D gAlbedoSpec;

// ���C�g�̍ő吔
const int MaxLightNum = 32;
// ���C�g�̔z��
uniform Light uLights[MaxLightNum];

// ���_(���[���h���W)
uniform vec3 uViewPos;

void main()
{
	// G-buffer���烉�C�e�B���O�ɕK�v�ȃf�[�^���擾����
    // G-Buffer�̓X�N���[���Ɠ����傫���Ȃ̂ŁA
    // 2D�X�N���[���`��p�̃e�N�X�`�����W��G-Buffer���Q�Ƃł���
	vec3  FragPos  = texture(gPosition, fragTexCoord).rgb;
	vec3  Normal   = texture(gNormal, fragTexCoord).rgb;
	vec3  Albedo   = texture(gAlbedoSpec, fragTexCoord).rgb;
	float Specular = texture(gAlbedoSpec, fragTexCoord).a;

	// �ʏ�ʂ�Ƀ��C�e�B���O�v�Z���s��
	vec3 lighting = Albedo * 0.1;
	vec3 viewDir = normalize(uViewPos - FragPos);

	// ���C�g�̌������[�v
	for(int i = 0; i<MaxLightNum; ++i)
	{
		// �f�B�t���[�Y���C�e�B���O
		vec3 lightDir = normalize(uLights[i].Position - FragPos);
		vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Albedo * uLights[i].Color;
		
		// ���C�e�B���O���ʂ����Z
		lighting += diffuse;
	}
	// �ŏI�J���[
	FragColor = vec4(lighting, 1.0);
}