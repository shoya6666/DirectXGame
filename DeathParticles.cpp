#include "DeathParticles.h"
#include <cassert>
#include "MeMath.h"
#include <numbers>
#include <algorithm>

using namespace KamataEngine;
using namespace MathUtility;

void DeathParticles::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::Vector3& position) {
	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	for (WorldTransform& worldTransform : worldTransforms_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathParticles::Update() {
	//ワールド変換の更新
	for (WorldTransform& worldTransform : worldTransforms_) {
		//  アフィン変換行列の作成
		worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
		// 定数バッファに転送する
		worldTransform.TransferMatrix();
	}

	for (uint32_t i = 0; i < kNumParticles; i++) {
		//基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		//回転角を計算する
		float angle = kAngleUint * i;
		//ｚ軸まわりの回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		//基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		//移動処理
		worldTransforms_[i].translation_ += velocity;
	}

	//カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	//存読時間の上限に達成したら
	if (counter_ >= kDuration) {
		counter_ = kDuration;
		//終了扱いにする
		isFinished_ = true;
	}

	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	color_.w = std::clamp(1.0f - counter_ / kDuration, 0.0f, 1.0f);
	//色変更オブジェクトに色の数値を変更する
	objectColor_.SetColor(color_);
}

void DeathParticles::Draw() {
	
	//終了なら何もしない
	if (isFinished_) {
		return;
	}

	for (auto& worldTransforms : worldTransforms_) {
		model_->Draw(worldTransforms, *camera_, &objectColor_);
	}
}
