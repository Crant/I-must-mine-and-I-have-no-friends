#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define DBG_NEW dbgNew ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW dbgNew
#endif

#include "GameEngine.h"

int main()
{
	IMM::GameEngine gameEngine = IMM::GameEngine();

	if (gameEngine.Construct(768, 512, 2, 2, false))
		gameEngine.Start();

	_CrtDumpMemoryLeaks();

	return 0;
}
