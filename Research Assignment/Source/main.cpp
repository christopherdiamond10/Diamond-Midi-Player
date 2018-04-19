#include "GameEngine.h"

void main()
{
	// Make Game
	GameEngine* pGame = new GameEngine();
	
	// Call OnLoad throughout the Engine
	pGame->OnLoad();

	// Run Game
	pGame->Run();

	// Call Unload throughout the Engine
	pGame->OnUnload();

	// Clean Game
	delete pGame;
}