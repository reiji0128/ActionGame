#version 330 core

uniform vec3 lightColor;

// �J���[�o�b�t�@�ւ̏o�͐F
out vec4 outColor;

void main()
{           
    outColor = vec4(lightColor, 1.0);
}