#pragma once
#include "KamataEngine.h"
#include "Mymath.h"
#include <array>
#include <numbers>

class DeathParticles {
public:
	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	void Update();

	void Draw();

	static inline const uint32_t kNumParticles = 8;

	static inline const float kDuration = 0.6f;
	static inline const float kSpeed = 0.3f;
	static inline const float kAnglUnit = 2.0f * std::numbers::pi_v<float> / kNumParticles;
	
	std::array<KamataEngine::WorldTransform, kNumParticles> worldTransforms_;

	//終了フラグ
	bool isFinished_ = false;
	//経過時間カウント
	float counter_ = 0.0f;

	KamataEngine::ObjectColor objectColor_;

	KamataEngine::Vector4 color_;

private:
	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	KamataEngine::Camera* camera_ = nullptr;
};
