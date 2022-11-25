#version 330 core

layout (location = 0) in vec3 inPos;

uniform mat4 model;
uniform mat4 uView;
uniform mat4 uProjection;

out vec2 TexCoords;

void main()
{
	vec4 pos = vec4(inPos,1.0);
	pos = pos * model;
	pos = pos * uView * uProjection;
	gl_Position = pos;
	
	// ���K���f�o�C�X���W���,G-Buffer�̃e�N�X�`�����W�ɕϊ�
	pos.x = pos.x / pos.w * 0.5f + 0.5f;
	pos.y = pos.y / pos.w * 0.5f + 0.5f;
	TexCoords = pos.xy;
}