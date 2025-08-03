#include "GameScene.h"
#include "KamataEngine.h"
#include "TitleScene.h"
#include <Windows.h>

using namespace KamataEngine;

TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;

enum class Scene {
	kUnknown, 

	kTitle, // タイトルシーン
	kGame,  // ゲームシーン
};

// 現在のシーン
Scene scene = Scene::kUnknown;

/// <summary>
/// シーンの切り替え
/// </summary>
static void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;

			// 前のシーンの開放
			delete titleScene;
			titleScene = nullptr;

			// 新しいシーンの初期化と生成
			gameScene = new GameScene();
			gameScene->Initialize();
		}

		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;

			// 前のシーンの開放
			delete gameScene;
			gameScene = nullptr;

			// 新しいシーンの初期化と生成
			titleScene = new TitleScene();
			titleScene->Initialize();
		}

		break;
	}
}

/// <summary>
/// シーンの更新
/// </summary>
static void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();

		break;
	case Scene::kGame:
		gameScene->Update();

		break;
	}
}

/// <summary>
/// シーンの描画
/// </summary>
static void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();

		break;
	case Scene::kGame:
		gameScene->Draw();

		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int){	// エンジンの初期化
	KamataEngine::Initialize(L"GC2C06タナカ_ショウヤ_AL3");

	// DirectXの機能取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// タイトルシーンの取得・初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene();
	titleScene->Initialize();

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		// シーンの切り替え
		ChangeScene();

		// シーンの更新
		UpdateScene();

		// 描画開始
		dxCommon->PreDraw();

		// シーンの描画
		DrawScene();

		// 描画終了
		dxCommon->PostDraw();
	}

	// タイトルシーンの解放
	delete titleScene;
	titleScene = nullptr;

	// ゲームシーンの解放
	delete gameScene;
	gameScene = nullptr;

	KamataEngine::Finalize();

	return 0;
}
