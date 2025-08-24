#include "TitleScene.h"
#include "MeMath.h"
#include <numbers>

using namespace KamataEngine;


TitleScene::~TitleScene() {

	delete model_;
	delete modelPlayer_;
	//フェード
	delete fade_;

}

void TitleScene::Initialize() {
	model_ = Model::CreateFromOBJ("titleFont");
	modelPlayer_ = Model::CreateFromOBJ("player");


	camera_.Initialize();

	worldTransform_.Initialize();
	worldTransform_.scale_ = {2, 2, 2};
	worldTransform_.translation_={0, 8, 0};

	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {10, 10, 10};
	worldTransformPlayer_.translation_ = {0, -8, 0};
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;

	fade_ = new Fade();
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void TitleScene::Update() {

	switch (phase_) {
	case TitleScene::Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			phase_ = Phase::kFadeOut;
			fade_->Start(Fade::Status::FadeOut, 1.0f);
		}
		break;
	case TitleScene::Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case TitleScene::Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}
		break;
	}

	worldTransform_.matWorld_ = 
		MakeAffineMatrix(worldTransform_.scale_,
		worldTransform_.rotation_,
		worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	rotate += 0.1f;

	worldTransformPlayer_.rotation_.y = sin(rotate) + std::numbers::pi_v<float>;
	
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());

	model_->Draw(worldTransform_, camera_);
	modelPlayer_->Draw(worldTransformPlayer_, camera_);

	fade_->Draw();
}


