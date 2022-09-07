#version 330 core

layout(location = 0) out vec4 FragBuffer;          // HDR�o�b�t�@�ɏo��
layout(location = 1) out vec4 BrightBuffer;        // ���P�x�o�b�t�@�ɏo��

// ���C�g�̐F
uniform vec3 color;

// �P�x
uniform float luminance;

out vec4 outColor;

void main()
{
	vec3 result;
	result = color * luminance;

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