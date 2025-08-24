#pragma once
#include "KamataEngine.h"
#include <array>
#include <numbers>

class DeathParticles {
public:

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	// 更新
	void Update();

	// 描画
	void Draw();

	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;

	//存続時間
	static inline const float kDuration = 0.8f;

	//移動の速さ
	static inline const float kSpeed = 0.1f;

	//分割した1個分の角度
	static inline const float kAngleUint = 2.0f * std::numbers::pi_v<float> / kNumParticles;

	bool IsFinished() const { return isFinished_; }
	
	//経過時間カウント
	float counter_ = 0.0f;

	//色変更オブジェクト
	KamataEngine::ObjectColor objectColor_;

	//色の数値
	KamataEngine::Vector4 color_;

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// 終了フラグ
	bool isFinished_ = false;
};
