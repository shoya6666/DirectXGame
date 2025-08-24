#pragma once
#include"KamataEngine.h"
#include <numbers>
#include "MeMath.h"

class Player;

class Enemy {
public:

	// 初期化
	void Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position);

	//デストラクタ
	~Enemy();

	// 更新
	void Update();

	// 描画
	void Draw();

	//歩行の速さ
	static inline const float kWalkSpeed = 0.1f;

	// 最初の角度
	static inline const float kWalkMotionAngleStart = static_cast<float>(30.0 * std::numbers::pi / 180.0);

	// 最後の角度
	static inline const float kWalkMotionAngleEnd = static_cast<float>(-30.0 * std::numbers::pi / 180.0);

	// アニメーションの時間
	static inline const float kWalkMotionTime = 1.0f;

	//経過時間
	float walkTimer_ = 0.0f;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	// ワールド座標を取得
	KamataEngine::Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();

	//衝突応答
	void OnCollision(const Player* player);

private:

	// ワールド変換データ
	KamataEngine::WorldTransform worldTransform_;

	// モデル
	KamataEngine::Model* model_ = nullptr;

	//カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// 速度
	KamataEngine::Vector3 velocity_ = {};

};
