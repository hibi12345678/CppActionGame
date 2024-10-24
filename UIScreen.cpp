// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#include "UIScreen.h"
#include "Texture.h"
#include "Shader.h"
#include "Game.h"
#include "Renderer.h"
#include "Font.h"
#include <SOIL/SOIL.h>
#include "AudioSystem.h"
UIScreen::UIScreen(Game* game)
	:mGame(game)
	,mTitle(nullptr)
	,mBackground(nullptr)
	,mTitlePos(0.0f, 200.0f)
	,mNextButtonPos(0.0f, 100.0f)
	,mNextTextPos(0.0f, -100.0f)
	,mBGPos(0.0f, 150.0f)
	,mUIState(EActive)
{
	// Add to UI Stack
	mGame->PushUI(this);
	mFont = mGame->GetFont("Assets/Carlito-Regular.ttf");
	mButtonOn = mGame->GetRenderer()->GetTexture("Assets/Texture/ButtonUIRed.png");
	mButtonOff = mGame->GetRenderer()->GetTexture("Assets/Texture/ButtonUI.png");
	
	

}

UIScreen::~UIScreen()
{
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
	}

	for (auto b : mButtons)
	{
		delete b;
	}	
	for (auto b : mStartButton)
	{
		delete b;
	}
	
	for (auto b : mText)
	{
		delete b;
	}
	mButtons.clear();
	mStartButton.clear();
	mText.clear();
}

void UIScreen::Update(float deltaTime)
{

}

void UIScreen::Draw(Shader* shader)
{
	

	// Draw background (if exists)
	if (mBackground)
	{
		DrawTexture(shader, mBackground, mBGPos);
	}
	// Draw title (if exists)
	if (mTitle)
	{		
		
		DrawTexture(shader, mTitle, mTitlePos);
		
	}

	// Draw buttons
	for (auto b : mButtons)
	{
		// Draw background of button
		Texture* tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
		
		DrawTexture(shader, tex, b->GetPosition());
		// Draw text of button
		DrawTexture(shader,b->GetNameTex(), b->GetPosition());
		

	}	
	// Draw buttons
	for (auto b : mStartButton)
	{
		// Draw background of button
		Texture* tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
		DrawTexture(shader, tex, b->GetPosition());
		// Draw text of button
		DrawTexture(shader,b->GetNameTex(), b->GetPosition());
		//b->SetName("Title");
		//DrawTexture(shader,b->GetNameTex(), b->GetPosition() + Vector2(0,100.0f));
		
		
	}
	// Draw buttons
	for (auto tex : mText)
	{
		
		DrawTexture(shader, tex, tex->GetPos());
		
	}


	// Override in subclasses to draw any textures
}

void UIScreen::ProcessInput(const uint8_t* keys)
{
	// Do we have buttons?
	if (!mButtons.empty())
	{
		// Get position of mouse
		int x, y;
		SDL_GetMouseState(&x, &y);
		// Convert to (0,0) center coordinates
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;
		
		// Highlight any buttons
		for (auto b : mButtons)
		{
			
			if (b->ContainsPoint(mousePos))
			{
				b->SetHighlighted(true);
			}
			else
			{
				b->SetHighlighted(false);
			}
		}

	}

}

void UIScreen::StartInput(const uint8_t* keys)
{
	
	// Do we have buttons?
	if (!mStartButton.empty())
	{
		
		// Get position of mouse
		int x, y;
		SDL_GetMouseState(&x, &y);
		// Convert to (0,0) center coordinates
		Vector2 mousePos(static_cast<float>(x), static_cast<float>(y));
		mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
		mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;
		
		for (auto b : mStartButton)
		{
			
			if (b->ContainsPoint(mousePos))
			{
				
				b->SetHighlighted(true);
			}
			else
			{
				// デバッグ用にサイズを出力
				
				b->SetHighlighted(false);
			}
		}
		
	}
}
void UIScreen::HandleKeyPress(int key)
{


	//if (currentState == EMainMenu ) 
	{
		
		switch (key)
		{
		case SDL_BUTTON_LEFT:
			
			if (!mStartButton.empty())
			{
				
				for (auto b : mStartButton)
				{
					
					if (b->GetHighlighted())
					{
						
						b->OnClick();
						
						break;
					}
				}

			}
			break;
		default:
			break;
		}
	}
	switch (key)
	{
	case SDL_BUTTON_LEFT:
		if (!mButtons.empty())
		{
			for (auto b : mButtons)
			{
				if (b->GetHighlighted())
				{
					b->OnClick();
					break;
				}
			}
		}
		break;
	default:
		break;
	}


}

void UIScreen::Close()
{
	mUIState = EClosing;
}

void UIScreen::SetTitle(const std::string& text,
						const Vector3& color,
						int pointSize)
{
	// Clear out previous title texture if it exists
	if (mTitle)
	{
		mTitle->Unload();
		delete mTitle;
		mTitle = nullptr;
	}
	
	mTitle = mFont->RenderText(text, color, pointSize);

}

void UIScreen::AddText(const std::string& text, Vector2 pos, int pointSize,
	const Vector3& color
	 )
{
	
	Texture* tex = new Texture();
	
	
	tex = mFont->RenderText(text, color, pointSize);
	tex->SetPos(pos);
	mText.emplace_back(tex);
	
	
}
void UIScreen::AddButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mButtonOn->GetWidth()), 
		static_cast<float>(mButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, mNextButtonPos, dims);
	mButtons.emplace_back(b);
	
	// Update position of next button
	// Move down by height of button plus padding
	mNextButtonPos.y -= mButtonOff->GetHeight() +20.0f;
	
}

void UIScreen::StartButton(const std::string& name, std::function<void()> onClick)
{
	Vector2 dims(static_cast<float>(mButtonOn->GetWidth()),
		static_cast<float>(mButtonOn->GetHeight()));
	Button* b = new Button(name, mFont, onClick, Vector2(0.0f, 0.0f), dims);
	
	mStartButton.emplace_back(b);
	
	

}
void UIScreen::DrawTexture(class Shader* shader, class Texture* texture,
				 const Vector2& offset, float scale, bool flipY , int a)
{
	// Scale the quad by the width/height of texture
	// and flip the y if we need to

	if (a == 0) {
		float yScale = static_cast<float>(texture->GetHeight() * scale);
		if (flipY) { yScale *= -1.0f; }
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(texture->GetWidth()) * scale,
			yScale,
			1.0f);
		// Translate to position on screen
		Matrix4 transMat = Matrix4::CreateTranslation(
			Vector3(offset.x, offset.y, 0.0f));

		// Set world transform
		Matrix4 world = scaleMat * transMat;
		shader->SetMatrixUniform("uWorldTransform", world);
		// Set current texture
		texture->SetActive();
		// Draw quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
	else if (a == 1) {
		float yScale = static_cast<float>(texture->GetHeight() *1.3);
		if (flipY) { yScale *= -1.0f; }
		Matrix4 scaleMat = Matrix4::CreateScale(
			static_cast<float>(texture->GetWidth()) * scale,
			yScale,
			1.0f);
		// Translate to position on screen
		Matrix4 transMat = Matrix4::CreateTranslation(
			Vector3(offset.x, offset.y, 0.0f));

		// Set world transform
		Matrix4 world = scaleMat * transMat;
		shader->SetMatrixUniform("uWorldTransform", world);
		// Set current texture
		texture->SetActive();
		// Draw quad
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}



}

void UIScreen::SetRelativeMouseMode(bool relative)
{
	if (relative)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);
		// Make an initial call to get relative to clear out
		SDL_GetRelativeMouseState(nullptr, nullptr);
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}
}

Button::Button(const std::string& name, Font* font,
	std::function<void()> onClick,
	const Vector2& pos, const Vector2& dims)
	:mOnClick(onClick)
	,mNameTex(nullptr)
	,mFont(font)
	,mPosition(pos)
	,mDimensions(dims)
	,mHighlighted(false)
{
	SetName(name);

}

Button::~Button()
{
	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
	}
}

void Button::SetName(const std::string& name)
{
	mName = name;

	if (mNameTex)
	{
		mNameTex->Unload();
		delete mNameTex;
		mNameTex = nullptr;
	}
	mNameTex = mFont->RenderText(mName);
}

bool Button::ContainsPoint(const Vector2& pt) const
{

	bool no = pt.x < (mPosition.x - mDimensions.x / 2.0f) ||
		pt.x > (mPosition.x + mDimensions.x / 2.0f) ||
		pt.y < (mPosition.y - mDimensions.y / 2.0f) ||
		pt.y > (mPosition.y + mDimensions.y / 2.0f);

	return !no;
}

void Button::OnClick()
{
	
	// Call attached handler, if it exists
	if (mOnClick)
	{		
		mOnClick();
	}
}
