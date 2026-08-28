#include "Stage1.h"
#include <Actor/Player.h>
#include <Actor/Mouse.h>
#include <Actor/Cat.h>

void Stage1::OnInitialized()
{
	Level::OnInitialized();

	SpawnActor<Player>();
	SpawnActor<Mouse>(position, Craft::Color::White);
	SpawnActor<Cat>(catPos, Craft::Color::White);
}
