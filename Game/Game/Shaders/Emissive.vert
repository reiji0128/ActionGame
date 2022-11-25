#version 330 core

// Attribute 1 位置
layout(location = 0) in vec3 inPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

// 任意の頂点を出力
out vec2 fragTexCoord;

uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

void main()
{
	vec4 pos = vec4(inPos,1.0);
	pos = pos * uWorldTransform;
	gl_Position = pos * uViewProj;

	// テクスチャ座標をフラグメントシェーダーに渡す
	fragTexCoord = inTexCoord;
}