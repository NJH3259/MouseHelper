#include "Stage1.h"
#include <Actor/Player.h>
#include <Actor/Mouse.h>
#include <Actor/Cat.h>

void Stage1::OnInitialized()
{
	StageLevel::OnInitialized();

	SpawnActor<Player>();
	SpawnActor<Mouse>(position, Craft::Color::White);
	SpawnActor<Cat>(catPos1, Craft::Color::White);
	SpawnActor<Cat>(catPos2, Craft::Color::White);
}

void Stage1::ResetLevel()
{
	StageLevel::ResetLevel();

	hasInitialized = false;

	actorList.clear();

	OnInitialized();
}
