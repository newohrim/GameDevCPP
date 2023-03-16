#define SDL_MAIN_HANDLED

#include "QuadtreeDemo.h"

int main(int argc, char* argv[])
{
	QuadtreeDemo game;
	bool success = game.Initialize(600, 600);
	if (success) 
	{
		game.RunLoop();
	}
	game.Shutdown();

	return 0;
}