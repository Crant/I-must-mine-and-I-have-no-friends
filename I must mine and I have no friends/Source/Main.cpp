//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "GameEngine.h"

int main()
{
	IMM::GameEngine gameEngine = IMM::GameEngine();

	if (gameEngine.Construct(768, 512, 2, 2, false))
		gameEngine.Start();

	//_CrtDumpMemoryLeaks();

	return 0;
}
