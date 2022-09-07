#pragma once
#include "MeshComponent.h"
#include "Math.h"
#include <string>
#include "ShaderTag.h"

// 前方宣言
class GameObject;
class SkeletalMeshComponent;
class Shader;

class AttachMeshComponent : public MeshComponent
{
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="owner">ゲームオブジェクトのポインタ</param>
	/// <param name="skelMeshComp">アタッチ先のゲームオブジェクトのスケルタルメッシュのポインタ</param>
	/// <param name="boneName">アタッチ先のボーンの名前</param>
	/// <param name="shaderTag">適用するシェーダーのタグ</param>
	AttachMeshComponent(GameObject* owner,SkeletalMeshComponent* skelMeshComp,const char* boneName,ShaderTag shaderTag);
	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~AttachMeshComponent();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="shader">使用するシェーダークラスのポインタ</param>
	void Draw(Shader* shader) override;

// セッター //

	/// <summary>
	/// 差分の平行移動行列を設定
	/// </summary>
	/// <param name="pos">位置</param>
	void SetOffsetPosition(const Vector3& pos);

	/// <summary>
	/// 差分の回転行列を設定
	/// </summary>
	/// <param name="rot">角度</param>
	void SetOffsetRotation(const Vector3& rot);

protected:
	//ボーン名から取得したインデックス
	int mBoneIndex;

	//アタッチ先のボーン名
	const char* mBoneName;

	//オフセット位置
	Matrix4 mPositionMat;

	//オフセット角度
	Matrix4 mRotationMat;

	// スケルトンメッシュのポインタ
	SkeletalMeshComponent* mSkelMesh;
};