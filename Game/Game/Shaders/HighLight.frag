#version 330 core

layout(location = 0) out vec4 FragBuffer;          // HDR�o�b�t�@�ɏo��
layout(location = 1) out vec4 BrightBuffer;        // ���P�x�o�b�t�@�ɏo��

uniform sampler2D uTexture;

in vec2 fragTexCoord;

void main()
{

	vec3 result = texture(uTexture,fragTexCoord).rgb * 1;

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