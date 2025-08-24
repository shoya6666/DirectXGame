#include "GameScene.h"
#include"MeMath.h"

using namespace KamataEngine;
using namespace MathUtility;

void GameScene::Initialize() {
	// 3dモデルの生成
	modelBlock_ = Model::CreateFromOBJ("block", true);

	modelSkydome_ = Model::CreateFromOBJ("Skydome", true);

	modelPlayer_ = Model::CreateFromOBJ("player", true);

	modelEnemy_ = Model::CreateFromOBJ("enemy", true);

	// skydomeの生成
	skydome_ = new Skydome();

	// skydomeの初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	// playerの生成
	player_ = new Player();

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);

	// playerの初期化
	player_->Initialize(modelPlayer_, &camera_,playerPosition);

	//enemy
	for (uint32_t i = 0; i < 5; i++) {
		// enemy の生成
		Enemy* newEnemy = new Enemy();
		// enemyの場所
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(15 + i, 18);
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	//// 3dモデルの生成

	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	// カメラの初期化
	camera_.Initialize();

	player_->SetMapChipField(mapChipField_);

	// デバックカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	//カメラコントローラーの生成
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->setTarget(player_);
	cameraController_->Reset();

	// 移動範囲の指定
	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

	//ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;

	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);

	//// 要素数
	//const uint32_t kNumBlockVertical = 10;   // 縦
	//const uint32_t kNumBlockHorizontal = 20; // 横

	//// ブロック1個分の幅
	//const float kBlockWidth = 1.0f;  // 縦
	//const float kBlockHeight = 1.0f; // 横

	//// 要素数を変更する
	//worldTransformBlocks_.resize(kNumBlockVertical);
	//for (uint32_t i = 0; i < kNumBlockVertical; i++) {
	//	// 1列の要素数を設定(横方向のブロック数)
	//	worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	//}

	//// ブロックの生成
	//for (uint32_t i = 0; i < kNumBlockVertical; i++) {
	//	for (uint32_t j = 0; j < kNumBlockHorizontal; j++) {
	//		if ((j % 2 == 0 && i % 2 == 0) || (j % 2 == 1 && i % 2 == 1)) {

	//			// ワールドトランスフォームの生成
	//			worldTransformBlocks_[i][j] = new WorldTransform();

	//			// ワールドトランスフォームの初期化
	//			worldTransformBlocks_[i][j]->Initialize();

	//			// x座標
	//			worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;

	//			// y座標
	//			worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
	//		}
	//	}
	//}

	GenerateBlocks();

}

GameScene::~GameScene() {
	// モデルの解放
	delete modelBlock_;
	modelBlock_ = nullptr;

	// skydomeの解放
	delete modelSkydome_;
	modelSkydome_ = nullptr;

	delete skydome_;
	skydome_ = nullptr;

	// モデルplayerの開放
	delete modelPlayer_;
	modelPlayer_ = nullptr;

	// playerの開放
	delete player_;
	player_ = nullptr;

	//マップの解放
	delete mapChipField_;

	//enemyの解放
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	//deathParticles_の解放
	delete deathParticles_;
	deathParticles_ = nullptr;

	// 箱の解放
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	// デバックカメラの解放
	delete debugCamera_;
	debugCamera_ = nullptr;

	//フェードの解放
	delete fade_;
}

void GameScene::Update() {
	
	fade_->Update();

	switch (phase_) {
	case GameScene::Phase::kPlay:

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				// ブロックの穴あきを許容
				if (!worldTransformBlock) {
					continue; // nullチェック
				}

				// アフィン変換行列の作成
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			}
		}

		// skydomeのUPdate
		skydome_->Update();

		// playerのUPdate
		player_->Update();

		// デバックカメラの更新
		debugCamera_->Update();

		cameraController_->Update();

		ChangePhase();

		// enemyの更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// 全ての当たり判定を行う
		CheckAllCollisions();

		#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_D)) {
			isDebugCameraActive_ = !isDebugCameraActive_; // デバックカメラの有効無効を切り替え
		}
#endif

		// カメラの更新
		if (isDebugCameraActive_) {
			// デバックカメラの更新
			debugCamera_->Update();

			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロダクションの転送
			camera_.TransferMatrix();
		} else {
			// ビュープロダクション行列の更新と転送
			camera_.UpdateMatrix();
			camera_.matView = cameraController_->GetViewProjection().matView;
			camera_.matProjection = cameraController_->GetViewProjection().matProjection;

			camera_.TransferMatrix();
		}

		break;
	case GameScene::Phase::kDeath:

		// skydomeのUPdate
		skydome_->Update();

		// enemyの更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// deathParticles_の更新
		if (deathParticles_!=nullptr) {
			deathParticles_->Update();
		}

		
		// デバックカメラの更新
		debugCamera_->Update();

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				// ブロックの穴あきを許容
				if (!worldTransformBlock) {
					continue; // nullチェック
				}

				// アフィン変換行列の作成
				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			}
		}

		break;
	}
	if (deathParticles_ != nullptr && deathParticles_->IsFinished()) {
		finished = true;
	}
}

void GameScene::Draw() {

	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// 3dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());

	// 箱の描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			// ブロックの穴あきを許容
			if (!worldTransformBlock) {
				continue; // nullチェック
			}
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	skydome_->Draw();

	if (!player_->IsDead()) {
		player_->Draw();
	}
	

	//enemyの描画
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
	
	//deathParticles_の描画
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	// 3Dモデルの描画後処理
	Model::PostDraw();

	fade_->Draw();
}


void GameScene::GenerateBlocks() { 
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical(); 
	uint32_t numBlockHorizontal=mapChipField_->GetNumBlockHorizontal();
	
	worldTransformBlocks_.resize(numBlockVirtical);
		for (uint32_t i = 0; i < numBlockVirtical; ++i) {
			worldTransformBlocks_[i].resize(numBlockHorizontal);
		}

		for (uint32_t i = 0; i < numBlockVirtical; i++) {
		    for (uint32_t j = 0; j < numBlockHorizontal; j++) {
			    if (mapChipField_->GetmapChiptypeByIndex(j,i)==MapChipType::kBlock) {
				    WorldTransform* worldTransform = new WorldTransform();
				    worldTransform->Initialize();
				    worldTransformBlocks_[i][j] = worldTransform;
				    worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			    }
			}
		}
}

void GameScene::ChangePhase() {

	switch (phase_) {
	case GameScene::Phase::kPlay:

		if (player_->isDead_) {

			// 死亡演出
			phase_ = Phase::kDeath;

			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			// パーティクル生成
			deathParticles_ = new DeathParticles();

			// 3Dモデルの設定
			modelDeathParticles_ = Model::CreateFromOBJ("deathParticle", true);

			deathParticles_->Initialize(modelDeathParticles_, &camera_, deathParticlesPosition);
		}

		break;
	case GameScene::Phase::kDeath:

		if (deathParticles_!=nullptr&&deathParticles_->IsFinished()) {
			finished = true;
		}

		break;
	}
}

void GameScene::CheckAllCollisions() {
	//判定対象1と2の座標
	AABB aabb1, aabb2;

	//自キャラの座標
	aabb1 = player_->GetAABB();

	//自キャラと散弾全ての当たり判定
	for (Enemy* enemy : enemies_) {
		//散弾の座標
		aabb2 = enemy->GetAABB();

		//AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			//自キャラの衝突時間数を呼び出す
			player_->OnCollision(enemy);
			//敵弾の衝突時のコールバックを呼び出す
			enemy->OnCollision(player_);
		}
	}
	
}

