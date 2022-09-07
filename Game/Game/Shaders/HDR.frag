#version 330 core

// uv���W
in vec2 fragTexCoord;

// �o�̓J���[
out vec4 outColor;

uniform sampler2D uHDRBuffer;
// �I�o
uniform float exposure;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(uHDRBuffer,fragTexCoord).rgb;

	// �I�o�g�[���}�b�s���O
	vec3 mapped = vec3(1.0) -exp(-hdrColor * exposure);
	// �K���}�R���N�V����
	mapped = pow(mapped,vec3(1.0 / gamma));

	outColor = vec4(mapped,1.0);
}
