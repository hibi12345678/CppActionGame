// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#pragma once
#include "UIScreen.h"
#include <vector>

class HUD : public UIScreen
{
public:
	// (Lower draw order corresponds with further back)
	HUD(class Game* game);
	~HUD();

	void Update(float deltaTime) override;
	void Draw(class Shader* shader) override;
	enum State {
		EMainMenu,
		EGameplay,
		EPaused,
		EQuit
	};
	void AddTargetComponent(class TargetComponent* tc);
	void RemoveTargetComponent(class TargetComponent* tc);
	State currentState;
	
protected:
	void UpdateRadar(float deltaTime);
	class Game* mGame; // ゲームインスタンスのポインタを保持
	class Texture* mHealthBar;
	class Texture* mRadar;
	class Texture* mCrosshair;
	class Texture* mCrosshairEnemy;
	class Texture* mBlipTex;
	class Texture* mRadarArrow;
	class Texture* mTitle;
	class Texture* mStaminaBar;

	// All the target components in the game
	std::vector<class TargetComponent*> mTargetComps;
	// 2D offsets of blips relative to radar
	std::vector<Vector2> mBlips;
	// Adjust range of radar and radius
	float mRadarRange;
	float mRadarRadius;
	// Whether the crosshair targets an enemy
	bool mTargetEnemy;
	
};
