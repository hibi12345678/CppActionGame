#include "ItemMenu.h"
#include "Game.h"
#include "DialogBox.h"
#include <SDL/SDL.h>
#include "UIScreen.h"
#include "HUD.h"
#include "Renderer.h"
#include "AudioSystem.h"

ItemMenu::ItemMenu(Game* game)
	:UIScreen(game)
	
{
	mGame->SetState(Game::EItem);
	SDL_ShowCursor(SDL_ENABLE);
	SetRelativeMouseMode(false);
	SetTitle("Item");
	HUD* hudInstance = mGame->GetHUD();
	Renderer* r = mGame->GetRenderer();
	r->SetAmbientLight(Vector3(0.2, 0.2, 0.2));
	
	
	
	if (hudInstance->GetItemNum() == 0) {
		AddText("SwordText", Vector2(0.0f, -160.0f), 30);
	}
	else if (hudInstance->GetItemNum() == 1) {
		AddText("TorchText", Vector2(0.0f, -160.0f), 30);
	}
	else if (hudInstance->GetItemNum() == 2) {
		AddText("BowText", Vector2(0.0f, -160.0f), 30);
	}
	else if (hudInstance->GetItemNum() == 3) {
		AddText("BombText", Vector2(0.0f, -160.0f), 30);
	}

	ItemButton("SwordText", 0, [this, hudInstance]() { 
		CloseText();
		AddText("SwordText", Vector2(0.0f, -160.0f), 30);
		hudInstance->SetItemNum(0);
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Equipped");
		});
	ItemButton("TorchText", 1, [this, hudInstance]() {
		CloseText();
		AddText("TorchText", Vector2(0.0f, -160.0f),30);
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Equipped");
		hudInstance->SetItemNum(1);
		});
	ItemButton("BowText", 2, [this, hudInstance]() {
		CloseText();
		AddText("BowText", Vector2(0.0f, -160.0f),30);
		hudInstance->SetItemNum(2); 
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Equipped");
		});
	ItemButton("BombText", 3, [this, hudInstance]() {
		CloseText();
		AddText("BombText", Vector2(0.0f, -160.0f),30);
		hudInstance->SetItemNum(3);
		mMusicEvent = mGame->GetAudioSystem()->PlayEvent("event:/Equipped");
		});
}

ItemMenu::~ItemMenu()
{
	mGame->SetState(Game::EGameplay);
	Renderer* r = mGame->GetRenderer();
	r->SetAmbientLight(Vector3(0.4000000059604645,
		0.4000000059604645,
		0.4000000059604645));
	// Optionally, ensure the cursor is explicitly disabled
	SDL_ShowCursor(SDL_DISABLE);
	// Enable relative mouse mode for camera look
	SDL_SetRelativeMouseMode(SDL_TRUE);
	// Make an initial call to get relative to clear out
	SDL_GetRelativeMouseState(nullptr, nullptr);

}

void ItemMenu::HandleKeyPress(int key)
{
	UIScreen::HandleKeyPress(key);
	if (key == SDLK_TAB || key == SDLK_ESCAPE)
	{
		Close();
	}
}