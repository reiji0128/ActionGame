#version 330 core

// uv���W
in vec2 fragTexCoord;

// �o�͐F
out vec4 FragColor;

struct GaussBlurParam
{
	int SampleCount;
	vec3 Offset[16];
};

uniform GaussBlurParam uParam;

uniform sampler2D uBlursource;



void main()
{
	vec4 result = vec4(0.0f);
	for(int i = 0; i < uParam.SampleCount;i++)
	{
	   // �K�E�X�̏d��(uParam.Offset[i])�Ɋ�Â��ăe�N�X�`���J���[������		
       result += uParam.Offset[i].z * texture(uBlursource,fragTexCoord + uParam.Offset[i].xy);  
	}
	result.w = 1.0f;
	FragColor = result;
}