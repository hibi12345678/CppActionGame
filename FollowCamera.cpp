// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "FollowCamera.h"
#include "Actor.h"
#include "LevelLoader.h"
#include "AudioSystem.h"
#include "Game.h"
#include "AudioComponent.h"
#include <vector>
FollowCamera::FollowCamera(Actor* owner)
	:CameraComponent(owner)
	, mPitchSpeed(0.0f)
	, mMaxPitch(Math::Pi / 3.0f)
	, mPitch(0.0f)
{
}

void FollowCamera::Update(float deltaTime)
{
	// Call parent update (doesn't do anything right now)
	CameraComponent::Update(deltaTime);
	// Camera position is owner position
	Vector3 cameraPos = ComputeCameraPos() ;

	// Update pitch based on pitch speed
	mPitch += mPitchSpeed * deltaTime;
	// Clamp pitch to [-max, +max]
	mPitch = Math::Clamp(mPitch, -mMaxPitch * 0.7f, mMaxPitch*0.6f);
	// Make a quaternion representing pitch rotation,
	// which is about owner's right vector
	Quaternion q(mOwner->GetRight(), mPitch);

	// Rotate owner forward by pitch quaternion
	Vector3 viewForward = Vector3::Transform(
		mOwner->GetForward(), q);
	// Target position 100 units in front of view forward
	Vector3 target = mOwner->GetPosition() + viewForward * mTargetDist * 10.0f;
	// Also rotate up by pitch quaternion
	Vector3 up = Vector3::Transform(Vector3::UnitZ, q);

	// Create look at matrix, set as view
	Matrix4 view = Matrix4::CreateLookAt(cameraPos, target, up);
	SetViewMatrix(view);
	
	// Set listener attributes to the player's position, not the camera's
	Game* game = mOwner->GetGame();
	Vector3 playerPosition = mOwner-> GetPosition();
	game->GetAudioSystem()->SetListener(view,Vector3::Zero,1.0f,playerPosition);
}



void FollowCamera::LoadProperties(const rapidjson::Value& inObj)
{
	CameraComponent::LoadProperties(inObj);

	JsonHelper::GetVector3(inObj, "actualPos", mActualPos);
	JsonHelper::GetVector3(inObj, "velocity", mVelocity);
	JsonHelper::GetFloat(inObj, "horzDist", mHorzDist);
	JsonHelper::GetFloat(inObj, "vertDist", mVertDist);
	JsonHelper::GetFloat(inObj, "targetDist", mTargetDist);
	JsonHelper::GetFloat(inObj, "springConstant", mSpringConstant);
}

void FollowCamera::SaveProperties(rapidjson::Document::AllocatorType& alloc, rapidjson::Value& inObj) const
{
	CameraComponent::SaveProperties(alloc, inObj);

	JsonHelper::AddVector3(alloc, inObj, "actualPos", mActualPos);
	JsonHelper::AddVector3(alloc, inObj, "velocity", mVelocity);
	JsonHelper::AddFloat(alloc, inObj, "horzDist", mHorzDist);
	JsonHelper::AddFloat(alloc, inObj, "vertDist", mVertDist);
	JsonHelper::AddFloat(alloc, inObj, "targetDist", mTargetDist);
	JsonHelper::AddFloat(alloc, inObj, "springConstant", mSpringConstant);
}

Vector3 FollowCamera::ComputeCameraPos() const
{
	// Set camera position behind and above owner
	Vector3 cameraPos = mOwner->GetPosition();
	cameraPos -= mOwner->GetForward() * mHorzDist;
	cameraPos += Vector3::UnitZ * mVertDist;
	return cameraPos;
}
