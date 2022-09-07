#pragma once
#include <unordered_map>
#include <string>
#include <vector>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_types.h>
#include "Math.h"
#include "Tag.h"
#include "GameObject.h"

// 前方宣言
class Renderer;
class AudioManager;
class CameraObject;
class PhysicsWorld;
class SceneBase;
class GameObject;

class Game final
{
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Game();

	/// <summary>
	/// ゲーム実行処理
	/// </summary>
	void Run();

	/// <summary>
	/// ゲーム終了処理
	/// </summary>
	void Shutdown();

	/// <summary>
	/// 画面初期化設定
	/// </summary>
	/// <param name="screenWidth">スクリーンの幅</param>
	/// <param name="screenHeight">スクリーンの高さ</param>
	/// <param name="fullScreen">フルスクリーンにするか</param>
	/// <returns>
	/// true  : 初期化成功
	/// false : 初期化失敗
	/// </returns>
	bool Initialize(int screenWidth, int screenHeight, bool fullScreen = false);

// セッター //
	/// <summary>
	/// 開始シーンのセット
	/// </summary>
	/// <param name="pScene">SceneBaseのポインタ</param>
	void SetFirstScene(SceneBase* pScene) { mNowScene = pScene; }

	/// <summary>
	/// カメラオブジェクトのポインのタセット
	/// </summary>
	/// <param name="camera">セットするカメラオブジェクトのポインタ</param>
	void SetCameraObject(CameraObject* camera);

	/// <summary>
	/// カメラオブジェクトを非アクティブにセット
	/// </summary>
	/// <param name="camera">セットするカメラオブジェクトのポインタ</param>
	void SetInActiveCameraObject(CameraObject* camera);

	/// <summary>
	/// ゲームをシャットダウン
	/// </summary>
	void SetShutDown() { mIsRunning = false; }

// ゲッター //

	/// <summary>
	/// ゲームクラスのインスタンスを取得
	/// </summary>
	/// <returns>ゲームクラスのインスタンス</returns>
	static Game& GetInstance()
	{
		static Game instance;
		return instance;
	}

	/// <summary>
	/// Rendererポインタの取得
	/// </summary>
	/// <returns>Rendererのポインタ</returns>
	Renderer* GetRenderer() { return mRenderer; }


	/// <summary>
	/// SDLレンダラーの取得
	/// </summary>
	SDL_Renderer* GetSDLRenderer();

	/// <summary>
	/// デルタタイムの取得
	/// </summary>
	/// <returns>デルタタイム</returns>
	const float GetDeltaTime() { return mDeltaTime; }

	/// <summary>
	/// PhysicsWorldクラスのポインタを取得
	/// </summary>
	/// <returns>PhysicsWorldクラスのポインタ</returns> 
	PhysicsWorld* const GetPhysics() { return mPhysicsWorld; }

	/// <summary>
	/// AudioManagerクラスのポインタを取得
	/// </summary>
	/// <returns>AudioManagerクラスのポインタ</returns> 
	AudioManager* const GetAudio() { return mAudio; }

	/// <summary>
	/// タグからゲームオブジェクトを取得
	/// </summary>
	/// <param name="type">取得するゲームオブジェクトのタグ</param>
	/// <returns>ゲームオブジェクトのポインタ</returns>
	std::vector<GameObject*> const& GetGameObjects(Tag type);

	/// <summary>
	/// プレイヤーオブジェクトのポインタの取得
	/// </summary>
	/// <returns>プレイヤーオブジェクトのポインタ</returns>
	GameObject* GetPlayerObjects();

	/// <summary>
	/// タグの中にいる最初のゲームオブジェクトを返す
	/// </summary>
	/// <returns>最初のアクターのポインタ</returns>
	GameObject* GetFirstGameObject(Tag type);

	/// <summary>
	/// 注視点の座標の取得
	/// </summary>
	/// <returns>注視点の座標</returns>
	const Vector3& GetViewTargetPos();

	/// <summary>
	/// 視点の座標の取得
	/// </summary>
	/// <returns></returns>
	const Vector3& GetViewPos();

	/// <summary>
	/// ゲームオブジェクトの追加
	/// </summary>
	/// <param name="actor">追加するゲームオブジェクトのポインタ</param>
	void AddGameObject(GameObject* addObject);

	/// <summary>
	/// ゲームオブジェクトの削除
	/// </summary>
	/// <param name="actor">削除するゲームオブジェクトのポインタ</param>
	void RemoveGameObject(GameObject* removeObject);

	/// <summary>
	/// すべてのゲームオブジェクトの削除
	/// </summary>
	void RemoveAllGameObject();


	/// <summary>
	/// タイプのゲームオブジェクトは存在しているか？
	/// </summary>
	/// <param name="type">確認するアクターのタグ</param>
	/// <returns>アクタータイプのアクターの数</returns>
	bool IsExistGameObjectType(Tag type);

	/// <summary>
	/// ゲームオブジェクトIDからゲームオブジェクトへのポインタを検索する
	/// </summary>
	/// <param name="searchActorID">検索するゲームオブジェクトID</param>
	/// <returns>
	/// 該当するIDのゲームオブジェクトへのポインタ
	/// </returns>
	GameObject* FindGameObjectFromID(int searchActorID);

private:

	/// <summary>
	/// シングルトン
	/// </summary>
	Game();

	/// <summary>
	/// 入力処理
	/// </summary>
	void Input();

	/// <summary>
	///  描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// ゲーム全体の更新処理
	/// </summary>
	int Update();

	/// <summary>
	/// ゲームオブジェクトの更新処理
	/// </summary>
	void GameObjectUpdate();

	/// <summary>
	/// ゲームオブジェクトリスト表示（デバッグ用）
	/// </summary>
	void ShowGameObjectList();

private:
	// Rendererクラスのポインタ
	Renderer* mRenderer;

	// CameraObjectクラスのポインタ
	CameraObject* mActiveCamera;

	// SceneBaseのポインタ
	SceneBase* mNowScene;

	// PhysicsWorldクラスのポインタ(あたり判定システム)
	PhysicsWorld* mPhysicsWorld;

	// AudioManagerのポインタ(サウンド関連)
	AudioManager* mAudio;

	// ゲームループ回すか？
	bool mIsRunning;

	// ポーズモード
	bool mIsPauseMode;

	// 1フレームの経過時間（秒単位）
	float mDeltaTime;

	// 経過時間（ミリ秒単位）
	Uint32 mTicksCount;

	// 射影行列
	Matrix4 mViewMatrix;

	// ゲームオブジェクト追加準備用配列
	std::vector<GameObject*> mPendingGameObjects;

	// ゲームオブジェクト配列
	std::unordered_map<Tag, std::vector<GameObject*>> mGameObjects;
};


//Gameクラスのインスタンス
#define GAMEINSTANCE Game::GetInstance()

//Rendererクラスのポインタを取得
#define RENDERER     Game::GetInstance().GetRenderer()

//AudioManagerクラスのポインタを取得
#define AUDIO        Game::GetInstance().GetAudio()
