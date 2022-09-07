#version 330

// �|�C���g���C�g
struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in VS_OUT
{
	PointLight Light;
	vec2 TexCoords;
	vec3 FragPos;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
}fs_in;

out vec4 FragColor;

// �e�N�X�`���}�b�v
uniform sampler2D diffuseMap;
uniform sampler2D normalMap;

// �|�C���g���C�g�v�Z
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir);

void main()
{
	vec3 normal = texture(normalMap,fs_in.TexCoords).rgb;
	normal = normalize(normal * 2.0f - 1.0f);

	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec3 result;
	result = CalcPointLight(fs_in.Light,normal,viewDir);
	FragColor = vec4(result,0.0);
}

// �|�C���g���C�g�v�Z
vec3 CalcPointLight(PointLight light,vec3 normal,vec3 viewDir)
{
	vec3 color = vec3(texture(diffuseMap,fs_in.TexCoords));

	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);

	// �f�B�t���[�Y�V�F�[�f�B���O
	float diff = max(dot(lightDir,normal),0.0);

	// �X�y�L�����[�V�F�[�f�B���O
	vec3 halfVec = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal,halfVec),0.0),32.0);

	// ���ʂ̍���
	vec3 ambient = light.ambient * color;
	vec3 diffuse = light.diffuse * diff * color;
	vec3 specular = light.specular * spec;

	return (ambient + diffuse + specular);
}