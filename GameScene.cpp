#include "GameScene.h"
#include "MyMath.h"


using namespace KamataEngine;

void GameScene::Initialize() {

	sprite_ = Sprite::Create(textureHandle_, {100, 50});
	// model_ = Model::Create();
	model_ = Model::CreateFromOBJ("player");
	Enemymodel_ = Model::CreateFromOBJ("enemy");
	modelParticles_ = Model::CreateFromOBJ("deathParticle");
	// ワールドトランスフォーム
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	
	// 3Dモデルデータの生成
	modelBlock_ = Model::CreateFromOBJ("block");
	//
	cameraController_ = new CameraController();

	// サウンドデータハンドル
	// soundDataHandle_ = Audio::GetInstance()->LoadWave("fanfare.wav");
	// 音声再生
	// Audio::GetInstance()->PlayWave(soundDataHandle_);
	// voiceHandle_ = Audio::GetInstance()->PlayWave(soundDataHandle_, true);
	// ライン描画が参照するかめらを指定する
	PrimitiveDrawer::GetInstance()->SetCamera(&camera_);
	// デバッグカメラ生成
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	input_ = Input::GetInstance();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, &camera_);

	// マップ
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();
	// 自キャラの初期化
	KamataEngine::Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);
	player_->Initialize(model_, &camera_, playerPosition);
	
	// カメラコントローラの生成
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	cameraController_->setTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

	player_->SetMapChipField(mapChipField_);
	// 敵の初期化
	for (uint32_t i = 0; i < 2; i++) {
	
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(6 + i, 18);
		newEnemy->Initialize(Enemymodel_, &camera_, enemyPosition);

		enemies_.push_back(newEnemy);
	}

	//後で消す
	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(modelParticles_, &camera_, playerPosition);
}

void GameScene::Update() {
	// スプライトの今の座標を取得
	Vector2 position = sprite_->GetPosition();
	// 座標を｛２，１｝移動
	position.x += 2.0f;
	position.y += 1.0f;
	// 移動した座標をスプライトに反映
	sprite_->SetPosition(position);
	// スペースキーを押した瞬間
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 音声停止
		Audio::GetInstance()->StopWave(voiceHandle_);
	}
	// デバッグテキストの表示
#ifdef _DEBUG
	//// float3入力ボックス
	// ImGui::InputFloat3("inputFloat3", inputFloat3);
	//// float3スライダー
	// ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	//// デモウィンドウの表示の有効化
	// ImGui::ShowDemoWindow();
#endif
	debugCamera_->Update();
	player_->Update();
	// ブロックの更新
	for (std::vector<WorldTransform*>& worldTransformBlockkLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockkLine) {
			if (!worldTransformBlock) {
				continue;
			}
			worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
			// 定数バッファに転送する
			worldTransformBlock->TransferMatrix();
		}
	}
#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG
	// カメラの処理
	if (isDebugCameraActive_) {
		// デバックカメラの更新
		debugCamera_->Update();
		// デバックカメラのビュー行列
		camera_.matView = debugCamera_->GetCamera().matView;
		// デバックカメラのプロジェクション行列
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	} else {
		camera_.matView = cameraController_->GetViewProjection().matView;
		camera_.matProjection = cameraController_->GetViewProjection().matProjection;
		// ビュープロダクション行列の転送
		camera_.TransferMatrix();
	}
	skydome_->Update();
	cameraController_->Update();
	for (Enemy* enemy : enemies_) {
		enemy->Update();
	}
	ChecAllCollisions();

	if (deathParticles_) {
		deathParticles_->Update();
	}
	
}

void GameScene::Draw() {

	// 3Dモデル描画前処理
	Model::PreDraw();

	// 3Dモデル描画
	player_->Draw();
	for (std::vector<WorldTransform*>& worldTransformBlockkLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockkLine) {
			if (!worldTransformBlock) {
				continue;
			}
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
	skydome_->Draw();
	// 3Dモデル描画後処理
	Model::PostDraw();

	ChecAllCollisions();

	if (deathParticles_) {
		deathParticles_->Draw();
	}
}

GameScene::~GameScene() {
	delete sprite_;
	delete model_;
	delete debugCamera_;
	delete player_;
	delete modelBlock_;
	delete skydome_;
	delete modelSkydome_;
	delete mapChipField_;
	delete modelParticles_;
	for (std::vector<WorldTransform*>& worldTransformBlockkLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockkLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
}

void GameScene::GenerateBlocks() {

	// 要素数
	uint32_t numBlocksVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlocksHorizontal = mapChipField_->GetNumBlockHorizontal();

	// ブロック数1個分の横幅
	// const float kBlockWidth = 2.0f;
	// const float kBlockHeight = 2.0f;

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(numBlocksVirtical);
	// キューブの生成
	for (uint32_t i = 0; i < numBlocksVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlocksHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < numBlocksVirtical; ++i) {
		for (uint32_t j = 0; j < numBlocksHorizontal; ++j) {
			if (mapChipField_->GetmapChiptypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene::ChecAllCollisions() {

	{
		//判定対象1と2の座標
		AABB aabb1, aabb2;

		//自キャラの座標
		aabb1 = player_->GetAABB();

		//自キャラと敵弾すべての当たり判定
		for (Enemy* enemy : enemies_) {
			//敵弾の座標
			aabb2 = enemy->GetAABB();

			//AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				//自キャラの衝突時関数を呼び出す
				player_->OnCollision(enemy);
				//敵の衝突時関数を呼び出す
				enemy->OnCollision(player_);
			}
		}
	}

}
