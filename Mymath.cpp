#include "Mymath.h"
#include<cmath>
#include<numbers>
using namespace KamataEngine;
using namespace MathUtility;


float EaseInOut(float x1, float x2, float t) { 
	float easedT = -(std::cosf(std::numbers::pi_v<float> * t) - 1.0f) / 2.0f;
	return Lerp(x1, x2, easedT);
}

KamataEngine::Matrix4x4 MakeAffineMatrix(KamataEngine::Vector3& scale, KamataEngine::Vector3& rotation, KamataEngine::Vector3& translation) {

	Matrix4x4 matScale = MakeScaleMatrix(scale);

	Matrix4x4 matRotX = MakeRotateXMatrix(rotation.x);
	Matrix4x4 matRotY = MakeRotateYMatrix(rotation.y);
	Matrix4x4 matRotZ = MakeRotateZMatrix(rotation.z);
	Matrix4x4 matRot = matRotZ * matRotX * matRotY;

	Matrix4x4 matTrans = MakeTranslateMatrix(translation);

	Matrix4x4 matWorld = matScale * matRot * matTrans;

	return matWorld;

}

bool IsCollision(const AABB& aabb1, const AABB& aabb2) {
	
	return (aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && //X軸
	       (aabb1.min.y <= aabb2.max.y && aabb1.max.x >= aabb2.min.y) && //Y軸
			(aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z);  //Z軸

}
