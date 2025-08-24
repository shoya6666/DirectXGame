#pragma once
#include "KamataEngine.h"
#include "Skydome.h"
#include "player.h"
#include <vector>
#include "MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"
#include "Fade.h"

class GameScene {
public:

	//ゲームのフェーズ
	enum class Phase {
		kFadeIn,
		kPlay, //ゲームプレイ
		kDeath, //デス演出
		kFadeOut,
	};

	//ゲームの現在フェーズ
	Phase phase_;

	// 初期化
	void Initialize();

	// デストラクタ
	~GameScene();

	// 更新
	void Update();

	// 描画
	void Draw();


	void GenerateBlocks();

	void ChangePhase();

	// 3Dモデル
	KamataEngine::Model* modelBlock_ = nullptr;

	// 箱
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// カメラ
	KamataEngine::Camera camera_;

	//マップ
	MapChipField* mapChipField_;

	// デバックカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	// スカイドーム3Dモデル
	KamataEngine::Model* modelSkydome_ = nullptr;

	// skyDome
	Skydome* skydome_ = nullptr;

	// プレイヤー3Dモデル
	KamataEngine::Model* modelPlayer_ = nullptr;

	//player
	Player* player_ = nullptr;

	// カメラコントローラー
	CameraController* cameraController_ = nullptr;

	//enemy
	std::list<Enemy*> enemies_;

	//enemy3Dモデル
	KamataEngine::Model* modelEnemy_ = nullptr;

	//DeathParticles
	DeathParticles* deathParticles_ = nullptr;

	//パーティクルモデル
	KamataEngine::Model* modelDeathParticles_ = nullptr;

	bool IsFinished() const { return finished; }

	//全ての当たり判定を行う
	void CheckAllCollisions();

private:
	// 終了フラグ
	bool finished = false;

	Fade* fade_ = nullptr;
};