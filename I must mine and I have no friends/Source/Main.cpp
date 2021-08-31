#include "../Include/Game.h"

int main()
{
	if (IMM::Game::Main()->Construct(640, 480, 2, 2, false))
		IMM::Game::Main()->Start();

	return 0;
}
