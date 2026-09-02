#include "Stage1.h"
#include <Actor/Player.h>
#include <Actor/Mouse.h>
#include <Actor/Cat.h>
#include <Util/Util.h>

Stage1::Stage1()
{
	
}

void Stage1::OnInitialized()
{
	StageLevel::OnInitialized();

	grid = Util::LoadMapAsGrid("../Assets/Stage1.txt");

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

void Stage1::Tick(float deltaTime)
{
	StageLevel::Tick(deltaTime);

	Renderer::GetRenderer().SubmitTilemap(grid);
}
