#pragma once
#include "KamataEngine.h"

class Skydome {
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera);

	// デストラクタ
	~Skydome();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// ワールド変換データ
	KamataEngine::WorldTransform* worldTransform_ = nullptr;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;
};