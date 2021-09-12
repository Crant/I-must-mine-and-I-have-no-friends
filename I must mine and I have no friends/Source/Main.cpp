//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>

#include "GameEngine.h"

int main()
{
	if (GameEngine::Main()->Construct(640, 480, 2, 2, false))
		GameEngine::Main()->Start();

	//_CrtDumpMemoryLeaks();

	return 0;
}
