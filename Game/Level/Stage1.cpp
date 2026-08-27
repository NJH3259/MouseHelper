#include "Stage1.h"
#include <Actor/Player.h>
#include <Actor/Mouse.h>

void Stage1::OnInitialized()
{
	Level::OnInitialized();

	SpawnActor<Player>();
	SpawnActor<Mouse>(position, Craft::Color::White);
}
