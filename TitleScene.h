#pragma once
#include <KamataEngine.h>
#include"Fade.h"

class TitleScene {
public:

	enum class Phase {
		kFadeIn,
		kMain,
		kFadeOut,
	};

	~TitleScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();


	bool IsFinished() const { return finished_; }

private:

	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	KamataEngine::Camera camera_;

	KamataEngine::WorldTransform worldTransform_;
	KamataEngine::WorldTransform worldTransformPlayer_;

	float rotate = 0.0f;

	bool finished_ = false;

	//フェード
	Fade* fade_ = nullptr;

	Phase phase_ = Phase::kFadeIn;

};
