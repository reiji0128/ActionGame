#version 330 core
layout (location = 0) in vec3  inPosition;
layout (location = 1) in vec3  inNormal;
layout (location = 2) in uvec4 inSkinBones;
layout (location = 3) in vec4  inSkinWeights;
layout (location = 4) in vec2  inTexCoords;

// ワールド変換行列
uniform mat4 uWorldTransform; 
// ビュー射影行列
uniform mat4 uViewProj;
// 行列パレット
uniform mat4 uMatrixPalette[196];


out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

void main()
{
	// 位置を同時座標系に変換し
	vec4 pos = vec4(inPosition,1.0);

	// スキンの位置
	vec4 skinnedPos = (pos * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x;
	     skinnedPos += (pos * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y;
	     skinnedPos += (pos * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z;
	     skinnedPos += (pos * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	// ワールド空間に変換
	skinnedPos = skinnedPos * uWorldTransform;
	// ワールド空間の位置を保存
	FragPos = pos.xyz;

	// スキンの頂点の法線
	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = (skinnedNormal * uMatrixPalette[inSkinBones.x]) * inSkinWeights.x +
		            (skinnedNormal * uMatrixPalette[inSkinBones.y]) * inSkinWeights.y +
		            (skinnedNormal * uMatrixPalette[inSkinBones.z]) * inSkinWeights.z +
		            (skinnedNormal * uMatrixPalette[inSkinBones.w]) * inSkinWeights.w;

	Normal = (skinnedNormal * uWorldTransform).xyz;
	
	// UV座標をフラグメントシェーダに渡す
	TexCoords   = inTexCoords;

	gl_Position = skinnedPos * uViewProj;
}

