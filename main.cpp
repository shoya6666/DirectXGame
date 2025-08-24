#include "GameScene.h"
#include "KamataEngine.h"
#include <Windows.h>
#include "TitleScene.h"

using namespace KamataEngine;

GameScene* gameScene = nullptr;
TitleScene* titleScene = nullptr;

enum class Scene {

	kUnknown=0,

	kTitle,
	kGame,
};

//現在シーン
Scene scene = Scene::kUnknown;

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:

		if (titleScene->IsFinished()) {
			scene = Scene::kGame;

			delete titleScene;
			titleScene = nullptr;

			gameScene = new GameScene;
			gameScene->Initialize();
		}

		break;
	case Scene::kGame:

		if (gameScene->IsFinished()) {
			scene = Scene::kTitle;

			delete gameScene;
			gameScene = nullptr;

			titleScene = new TitleScene;
			titleScene->Initialize();
		}

		break;
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:

		titleScene->Update();

		break;
	case Scene::kGame:

		gameScene->Update();

		break;
	}
}

void DrawScene() {
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
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {
	// エンジンの初期化
	KamataEngine::Initialize(L"GC2C_06_タナカ_ショウヤ_AL3");

	// DirectXの機能取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();


	
	// タイトルシーンの取得・初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	// ゲームシーンの取得・初期化
	gameScene = new GameScene();
	gameScene->Initialize();


	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		// タイトルシーンの更新
		//
		ChangeScene();
		UpdateScene();

		// 描画開始
		dxCommon->PreDraw();

		// タイトルシーンの描画
		DrawScene();

		// 描画終了
		dxCommon->PostDraw();

	}

	// ゲームシーンの解放
	delete gameScene;
	gameScene = nullptr;
	
	// タイトルシーンの解放
	delete titleScene;
	titleScene = nullptr;

	KamataEngine::Finalize();

	return 0;
}


