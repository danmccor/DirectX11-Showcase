#include "Game.h"

int WINAPI WinMain(_In_ HINSTANCE hInst, _In_opt_ HINSTANCE hPrev, _In_ LPSTR lpCmd, _In_ int nCmdShow)
{
	//Create the game
	Game game;
	//initialise the game
	game.Initialise(hInst);
	//while the game is managing messages
	while (game.manageMessages()) {
		//update and render
		game.Update();
		game.Render();
	}
	//else exit
	return 0;
}
