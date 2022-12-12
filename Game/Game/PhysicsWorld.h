#pragma once
#include "Game.h"
#include "Tag.h"
#include <unordered_map>
#include <vector>

// 前方宣言
class ColliderComponent;
class Shader;

class PhysicsWorld
{
public:
	struct collidePairs
	{
		Tag pair1;
		Tag pair2;
	};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PhysicsWorld();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PhysicsWorld();

	/// <summary>
	/// コライダーの追加
	/// </summary>
	/// <param name="collider">追加するコライダー</param>
	void AddCollider(ColliderComponent* addCollider);

	/// <summary>
	/// コライダーの削除
	/// </summary>
	/// <param name="collider">削除するコライダー</param>
	void RemoveCollider(ColliderComponent* removeCollider);

	/// <summary>
	/// 当たり判定
	/// </summary>
	void Collision();

	/// <summary>
	/// デバッグモードの切り替え
	/// </summary>
	void ToggleDebugMode() { mBoolDebugMode = !mBoolDebugMode; }

	/// <summary>
	/// ボックスリスト表示（デバッグ用)
	/// </summary>
	void DebugShowBoxLists();

	
	/// <summary>
	/// デバッグ用ボックス表示
	/// </summary>
	void DebugShowBox();

	/// <summary>
	/// 片方だけリアクションを行うペアリストのクリア
	/// </summary>
	void ClearOneSidePair();

	/// <summary>
	/// 両方ともリアクションを行うペアリストのクリア
	/// </summary>
	void ClearDualPair();

	/// <summary>
	/// 同じグループ内でのペアリストのクリア
	/// </summary>
	void ClearSelfPair();

	/// <summary>
	/// すべてのペアリストのクリア
	/// </summary>
	void ClearAllPair();

	/// <summary>
	/// 線分が衝突しているかどうか
	/// </summary>
	/// <param name="line">線分</param>
	/// <param name="info">当たり判定情報</param>
	/// <returns>
	/// true  : 衝突している
	/// false : 衝突していない
	/// </returns>
	bool SegmentCast(const struct Line& line, struct CollisionInfo& info);

// セッター //

	/// <summary>
	/// 当たり判定ペアのセット（片方だけリアクション返すタイプ）
	/// </summary>
	/// <param name="noReactionType">リアクションしないオブジェクトのタグ</param>
	/// <param name="reactionType">リアクションするオブジェクトのタグ</param>
	void SetOneSideReactionCollisionPair(Tag noReactionType, Tag reactionType);

	/// <summary>
	/// 当たり判定ペアのセット（両方リアクション）
	/// </summary>
	/// <param name="reaction1">リアクションするオブジェクトのタグ1</param>
	/// <param name="reaction2">リアクションするオブジェクトのタグ2</param>
	void SetDualReactionCollisionPair(Tag reaction1, Tag reaction2);

	/// <summary>
	/// 同じグループ同士の当たり判定セット
	/// </summary>
	/// <param name="selfreaction">オブジェクトのタグ</param>
	void SetSelfReaction(Tag selfreaction);

private:
	/// <summary>
	/// 片方だけリアクションを行う当たり判定組み合わせの総当たりチェック
	/// </summary>
	/// <param name="cp">コライダーのペア</param>
	void OneReactionMatch(collidePairs cp);

	/// <summary>
	/// 両方ともリアクションを行う当たり判定組み合わせの総当たりチェック
	/// </summary>
	/// <param name="cp">コライダーのペア</param>
	void DualReactionMatch(collidePairs cp);

	/// <summary>
	/// 自分と同じ当たり判定グループの総組み合わせを行う
	/// </summary>
	/// <param name="type">同じグループのタグ</param>
	void SelfReactionMatch(Tag type);

	/// <summary>
	/// 描画用ボックスの頂点初期化
	/// </summary>
	void InitBoxVertices();

	/// <summary>
	/// 描画用四角の頂点初期化
	/// </summary>
	void InitSquareVertices();

	/// <summary>
	/// 描画用線分の頂点初期化
	/// </summary>
	void InitLineVertices();

	/// <summary>
	/// コリジョンの描画
	/// </summary>
	/// <param name="boxs">コライダーのコンポーネントの配列のポインタ</param>
	/// <param name="color">描画色</param>
	void DrawCollisions(std::vector<ColliderComponent*>& boxs, const Vector3& color);

	// デバッグモード
	bool mBoolDebugMode;

	// 片方だけリアクションを行う当たり判定ペアリスト
	std::vector<collidePairs> mOneSideReactions;

	// 両方ともリアクションを行う当たり判定ペアリスト
	std::vector<collidePairs> mDualReactions;

	// 同じグループ内での当たり判定を行うリスト
	std::vector<Tag> mSelfReactions;

	// 当たり判定ボックスのカラー
	std::vector<Vector3> mLineColors;

	// 当たり判定ボックスリスト
	std::unordered_map<Tag, std::vector<ColliderComponent*>> mColliders;

	// ボックス描画用のVAO 
	unsigned int mBoxVAO;   

	// 平面描画用のVAO
	unsigned int mSquareVAO;

	// ライン用VAO
	unsigned int mLineVAO;

	// ライン描画用シェーダー
	Shader* mLineShader;


};