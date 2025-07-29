#pragma once

#include"KamataEngine.h"

float EaseInOut(float x1, float x2, float t);

KamataEngine::Matrix4x4 MakeAffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& tranaslation);

//AABB
struct AABB {
	KamataEngine::Vector3 min;
	KamataEngine::Vector3 max;
};

//衝突判定
bool IsCollision(const AABB& aabb1, const AABB& aabb2);