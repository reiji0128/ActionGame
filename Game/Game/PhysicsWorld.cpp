﻿#include "PhysicsWorld.h"
#include <typeinfo>
#include <algorithm>
#include "Renderer.h"
#include "GameObject.h"
#include "BoxCollider.h"
#include "WallCollider.h"
#include "Collision.h"
#include "LineCollider.h"
#include "Shader.h"

/// <summary>
/// コンストラクタ
/// </summary>
PhysicsWorld::PhysicsWorld()
	:mBoolDebugMode(false)
{
	printf("PysicsWorld 作成\n");

	// コライダー描画用の頂点配列の初期化
	InitBoxVertices();
	InitSquareVertices();
	InitLineVertices();

	mLineShader = new Shader();
	mLineShader->Load("shaders/LineWorld.vert", "shaders/Line.frag");

	// 当たり判定ボックスのライン配列
	mLineColors.emplace_back(Color::White);
	mLineColors.emplace_back(Color::Red);
	mLineColors.emplace_back(Color::Green);
	mLineColors.emplace_back(Color::Blue);
	mLineColors.emplace_back(Color::Yellow);
	mLineColors.emplace_back(Color::LightYellow);
	mLineColors.emplace_back(Color::LightBlue);
	mLineColors.emplace_back(Color::LightPink);
	mLineColors.emplace_back(Color::LightGreen);
}

/// <summary>
/// デストラクタ
/// </summary>
PhysicsWorld::~PhysicsWorld()
{
	printf("PysicsWorld 破棄\n");
	delete mLineShader;
}

/// <summary>
/// コライダーの追加
/// </summary>
/// <param name="addCollider">追加するコライダー</param>
void PhysicsWorld::AddCollider(ColliderComponent *addCollider)
{
	Tag t = addCollider->GetTag();
	mColliders[t].emplace_back(addCollider);
}

/// <summary>
/// コライダーの削除
/// </summary>
/// <param name="removeCollider">削除するコライダー</param>
void PhysicsWorld::RemoveCollider(ColliderComponent* removeCollider)
{
	Tag t = removeCollider->GetOwner()->GetTag();
	// タグから検索して削除
	std::vector<ColliderComponent*>::iterator iter = std::find(mColliders[t].begin(), mColliders[t].end(), removeCollider);
	if (iter != mColliders[t].end())
	{
		mColliders[t].erase(iter);
		return;
	}
}

/// <summary>
/// 当たり判定
/// </summary>
void PhysicsWorld::Collision()
{
	// 片方だけリアクションを返す当たり判定
	for (auto reactionPair : mOneSideReactions)
	{
		OneReactionMatch(reactionPair);
	}

	// 両方リアクションを返す当たり判定
	for (auto reactionPair : mDualReactions)
	{
		DualReactionMatch(reactionPair);
	}

	// 自分と同じリストの当たり判定
	for (auto t : mSelfReactions)
	{
		SelfReactionMatch(t);
	}

}

/// <summary>
/// ボックスリスト表示（デバッグ用)
/// </summary>
void PhysicsWorld::DebugShowBoxLists()
{
	for(auto tag = Tag::Begin; tag != Tag::End; ++tag)
	{
		for (auto m : mColliders[tag])
		{
			Vector3 pos = m->GetOwner()->GetPosition();
			printf("%6d ", m->mGlobalID);
			printf("(% 7.2f,% 7.2f % 7.2f)-", pos.x, pos.y, pos.z);
			printf("[%p]\n", m->GetOwner());
		}
	}
}

/// <summary>
/// デバッグ用ボックス表示
/// </summary>
void PhysicsWorld::DebugShowBox()
{
	// デバッグモードか？
	if (!mBoolDebugMode)
	{
		return;
	}

	// AABB描画準備
	Matrix4 scale, trans, world, view, proj, viewProj;
	view = RENDERER->GetViewMatrix();
	proj = RENDERER->GetProjectionMatrix();
	viewProj = view * proj;
	mLineShader->SetActive();
	mLineShader->SetMatrixUniform("uViewProj", viewProj);

	// 当たり判定ボックス描画 tag毎に色を変えてすべてのリスト表示
	int colorCount = 0;
	size_t colorNum = mLineColors.size();
	for (auto t = Tag::Begin; t != Tag::End; ++t)
	{
		DrawCollisions(mColliders[t], mLineColors[colorCount % colorNum]);
		colorCount++;
	}
}

/// <summary>
/// 片方だけリアクションを行うペアリストのクリア
/// </summary>
void PhysicsWorld::ClearOneSidePair()
{
	mOneSideReactions.clear();
}

/// <summary>
/// 両方ともリアクションを行うペアリストのクリア
/// </summary>
void PhysicsWorld::ClearDualPair()
{
	mDualReactions.clear();
}


/// <summary>
/// 同じグループ内でのペアリストのクリア
/// </summary>
void PhysicsWorld::ClearSelfPair()
{
	mSelfReactions.clear();
}

/// <summary>
/// すべてのペアリストのクリア
/// </summary>
void PhysicsWorld::ClearAllPair()
{
	ClearOneSidePair();
	ClearDualPair();
	ClearSelfPair();
}

/// <summary>
/// 当たり判定ペアのセット（片方だけリアクション返すタイプ）
/// </summary>
/// <param name="noReactionType">リアクションしないオブジェクトのタグ</param>
/// <param name="reactionType">リアクションするオブジェクトのタグ</param>
void PhysicsWorld::SetOneSideReactionCollisionPair(Tag noReactionType, Tag reactionType)
{
	collidePairs cp;
	cp.pair1 = noReactionType;
	cp.pair2 = reactionType;

	mOneSideReactions.emplace_back(cp);
}

/// <summary>
/// 当たり判定ペアのセット（両方リアクション）
/// </summary>
/// <param name="reaction1">リアクションするオブジェクトのタグ1</param>
/// <param name="reaction2">リアクションするオブジェクトのタグ2</param>
void PhysicsWorld::SetDualReactionCollisionPair(Tag reaction1, Tag reaction2)
{
	collidePairs cp;
	cp.pair1 = reaction1;
	cp.pair2 = reaction2;
	mDualReactions.emplace_back(cp);
}

/// <summary>
/// 同じグループ同士の当たり判定セット
/// </summary>
/// <param name="selfreaction">オブジェクトのタグ</param>
void PhysicsWorld::SetSelfReaction(Tag selfreaction)
{
	mSelfReactions.emplace_back(selfreaction);
}

/// <summary>
/// 片方だけリアクションを行う当たり判定組み合わせの総当たりチェック
/// </summary>
/// <param name="cp">コライダーのペア</param>
void PhysicsWorld::OneReactionMatch(collidePairs cp)
{
	for (auto obj : mColliders[cp.pair1])
	{
		for (auto reactionObj : mColliders[cp.pair2])
		{
			if (obj->CollisionDetection(reactionObj))
			{
				reactionObj->GetOwner()->OnCollisionEnter(reactionObj, obj);
			}
		}
	}
}

/// <summary>
/// 両方ともリアクションを行う当たり判定組み合わせの総当たりチェック
/// </summary>
/// <param name="cp">コライダーのペア</param>
void PhysicsWorld::DualReactionMatch(collidePairs cp)
{
	for (auto reaction1 : mColliders[cp.pair1])
	{
		for (auto reaction2 : mColliders[cp.pair2])
		{
			if (reaction1->CollisionDetection(reaction2))
			{
				// それぞれリアクションを起こし、もう一方の衝突相手を渡す
				reaction1->GetOwner()->OnCollisionEnter(reaction1, reaction2);
			}
		}
	}
}

/// <summary>
/// 自分と同じ当たり判定グループの総組み合わせを行う
/// </summary>
/// <param name="type">同じグループのタグ</param>
void PhysicsWorld::SelfReactionMatch(Tag type)
{
	size_t size = mColliders[type].size();

	for (int i = 0; i < size; i++)
	{
		//自分自身の当たり判定は行わず、総当たりチェック
		for (int j = i + 1; j < size; j++)
		{
			if (mColliders[type][i]->CollisionDetection(mColliders[type][j]))
			{
				GameObject* obj1 = mColliders[type][i]->GetOwner();
				obj1->OnCollisionEnter(mColliders[type][i], mColliders[type][j]);
			}
		}
	}
}

/// <summary>
/// 描画用ボックスの頂点初期化
/// </summary>
void PhysicsWorld::InitBoxVertices()
{
	// ボックス頂点リスト
	float vertices[] = {
		0.0f, 0.0f, 0.0f,  // min
		1.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 1.0f,  // max
		0.0f, 1.0f, 1.0f,
	};
	// ボックスのラインリスト
	unsigned int lineList[] = {
		0,1,
		1,2,
		2,3,
		3,0,
		4,5,
		5,6,
		6,7,
		7,4,
		0,4,
		1,5,
		2,6,
		3,7,
	};
	unsigned int vbo, ebo;
	glGenVertexArrays(1, &mBoxVAO);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(mBoxVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineList), lineList, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

/// <summary>
/// 描画用四角の頂点初期化
/// </summary>
void PhysicsWorld::InitSquareVertices()
{
	// ボックス頂点リスト
	float vertices[] = {  // 奥下→奥上→手前上→手前下
		0.0f, 0.5f,-0.5f,
		0.0f, 0.5f, 0.5f,
		0.0f,-0.5f, 0.5f,
		0.0f,-0.5f,-0.5f,
		0.0f, 0.0f, 0.0f, // 法線表示
	   -1.0f, 0.0f, 0.0f,
	};
	// ボックスのラインリスト
	unsigned int lineList[] = {
		0,1,
		1,2,
		2,3,
		3,0,
		4,5
	};

	// 頂点配列 mSquareVAOの設定
	unsigned int vbo, ebo;
	glGenVertexArrays(1, &mSquareVAO);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(mSquareVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineList), lineList, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
}

/// <summary>
/// 描画用線分の頂点初期化
/// </summary>
void PhysicsWorld::InitLineVertices()
{
	// ライン頂点リスト
	float vertices[] =
	{
		0.0f,0.0f,0.0f,
		1.0f,0.0f,0.0f,
	};
	// ラインリスト
	unsigned int lineList[] =
	{
		0,1
	};
	// 頂点配列 mLineVAOの設定
	unsigned int vbo, ebo;
	glGenVertexArrays(1, &mLineVAO);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);
	glBindVertexArray(mLineVAO);
	{
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineList), lineList, GL_STATIC_DRAW);
	}
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

}

/// <summary>
/// コリジョンの描画
/// </summary>
/// <param name="boxs">コライダーのコンポーネントの配列のポインタ</param>
/// <param name="color">描画色</param>
void PhysicsWorld::DrawCollisions(std::vector<class ColliderComponent*>& collisions, const Vector3& color)
{
	Matrix4 scaleMat, posMat, rotMat, worldMat, slopeRot;
	Vector3 scale, pos;

	mLineShader->SetVectorUniform("uColor", color);
	for (auto item : collisions)
	{
		// Boxだった場合の描画
		if (item->GetColliderType() == ColliderTypeEnum::Box)
		{
			AABB box;
			Vector3 min, max;
			box = dynamic_cast<BoxCollider*>(item)->GetWorldBox();

			// ボックスのスケールと位置を取得
			min = box.mMin;
			max = box.mMax;
			scale = max - min;
			pos = min;

			scaleMat = Matrix4::CreateScale(scale);
			posMat = Matrix4::CreateTranslation(pos);

			worldMat = scaleMat * posMat;
			mLineShader->SetMatrixUniform("uWorld", worldMat);

			glBindVertexArray(mBoxVAO);
			glDrawElements(GL_LINES, 24, GL_UNSIGNED_INT, 0);
		}
		// Wallだった場合の描画 
		if (item->GetColliderType() == ColliderTypeEnum::Wall || 
			item->GetColliderType() == ColliderTypeEnum::Slope)
		{
			WallCollider* wallcol;
			Wall walldata;

			Vector3 scale; // 描画スケーリング係数
			Vector3 pos; // 描画位置
			Vector3 normal; // 壁法線

			// WallColliderと壁データ取得
			wallcol = dynamic_cast<WallCollider*>(item);
			walldata = wallcol->GetWall();
			normal = walldata.mNormal;
			// 4点の中点を求める
			for (int i = 0; i < 4; i++)
			{
				pos += walldata.mWallVertex[i];
			}
			pos = pos * 0.25f;

			// 行列
			scaleMat = Matrix4::CreateScale(walldata.mScale);
			rotMat   = Matrix4::CreateRotationZ(walldata.mZRotate);
			posMat   = Matrix4::CreateTranslation(pos);
			slopeRot = Matrix4::CreateRotationY(walldata.mSlopeAngle);

			worldMat = scaleMat * slopeRot * rotMat * posMat;
			mLineShader->SetMatrixUniform("uWorld", worldMat);

			glBindVertexArray(mSquareVAO);
			glDrawElements(GL_LINES, 10, GL_UNSIGNED_INT, 0);
		}
		// LINEだった場合
		if (item->GetColliderType() == ColliderTypeEnum::Line)
		{
			LineCollider* linecol;
			Line line;
			
			linecol = dynamic_cast<LineCollider*>(item);

			Vector3 scale;  // 描画スケーリング係数
			Vector3 pos;    // 描画位置
			line = linecol->GetLine();

			// 線分ベクトルと正規化線分方向ベクトル求める
			Vector3 lineVec = line.mLineEnd - line.mLineStart;;
			Vector3 lineDir = lineVec;
			lineDir.Normalize();

			//線分長からスケーリング行列作成
			float len = lineVec.Length();
			Matrix4 scaleMat = Matrix4::CreateScale(len, 0,0);

			// 線分から回転軸と回転角を求めクオータニオン作成
			Vector3 rotAxis;
			rotAxis = Vector3::Cross(lineDir, Vector3(1, 0, 0));
			float rotAngle = -1.0f * acosf(Vector3::Dot(rotAxis, lineDir));

			// クオータニオンから線分の方向に回転する行列を作成
			Quaternion q(rotAxis, rotAngle);
			Matrix4 rotMat = Matrix4::CreateFromQuaternion(q);

			// 平行移動成分
			Matrix4 posMat = Matrix4::CreateTranslation(line.mLineStart);

			
			worldMat = scaleMat * rotMat * posMat;
			mLineShader->SetMatrixUniform("uWorld", worldMat);

			//描画
			glBindVertexArray(mLineVAO);
			glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);
		}
	}
}
