#include "Skydome.h"
#define _USE_MATH_DEFINES
#include "math.h"

using namespace KamataEngine;
using namespace MathUtility;

void Skydome::Initialize(Model* model, Camera* camera) {

	assert(model);
	assert(camera);

	model_ = model;
	camera_ = camera;

	worldTransform_ = new WorldTransform();

	worldTransform_->translation_.x = 0.0f;
	worldTransform_->translation_.y = 0.0f;

	worldTransform_->Initialize();

	worldTransform_->rotation_.y = 90 * static_cast<float>(M_PI / 180);
}

Skydome::~Skydome() {
	delete worldTransform_;
	worldTransform_ = nullptr;
}

void Skydome::Update() {}

void Skydome::Draw() { model_->Draw(*worldTransform_, *camera_); }