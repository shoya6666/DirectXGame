#pragma once
#include "KamataEngine.h"

// マップチップタイプ
enum class MapChipType {
	kBlank, // 空白
	kBlock  // ブロック
};

struct MapChipData {

	std::vector<std::vector<MapChipType>> data;
};

class MapChipField {
public:
	// ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

	//マップチップデータ
	MapChipData mapChipData_;

	//インデックスセット
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	//座標からマップチップ番号を計算
	IndexSet GetMapChipIndexSetByPosition(const KamataEngine::Vector3& position);

	//範囲矩形
	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

	//ブロックの範囲取得関数
	Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

	void ResetMapChipData();

	void LoadMapChipCsv(const std::string& filePath);

	MapChipType GetmapChiptypeByIndex(uint32_t xIndex, uint32_t yIndex);

	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; };
	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; };
};