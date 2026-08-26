#include "Stage1.h"
#include <Actor/Player.h>

void Stage1::OnInitialized()
{
	Level::OnInitialized();

	SpawnActor<Player>();
	SpawnActor<Mouse>(position, Color::White);
}
