#pragma once

// 適用するシェーダーのタグ
enum class ShaderTag
{					    
	SPRITE,              // スプライト
	TILEMAP,             // タイルマップ
	PHONG,               // フォグ
	DEPTH_MAP,           // デプスマップ(メッシュ用)
	SKINNED_DEPTH_MAP,   // デプスマップ(スケルタルメッシュ用)
	SHADOW_MAP,          // シャドウマップ(メッシュ用)
	SKINNED_SHADOW_MAP,  // シャドウマップ(スケルタルメッシュ用)
	HDR,                 // HDR
	DOWNSAMPLING,        // ダウンサンプリング
	GAUSS_BLUR,          // ガウスブラー
	HDR_BLOOM_BLEND,     // HDRブルームブレンド
	NORMAL_MAP,		     // 法線マップ
	SCREEN_BUFFER,       // スクリーンバッファ
	SKYBOX,              // スカイボックス

	NONE,                // 適用するシェーダーなし
};