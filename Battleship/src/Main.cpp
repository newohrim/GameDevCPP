#define SDL_MAIN_HANDLED
#include "SBGame.h"

int main(int argc, char* argv[])
{
	SBGame game;
	bool success = game.Initialize();
	if (success) 
	{
		game.RunLoop();
	}
	game.Shutdown();

	return 0;
}