#include "Stage1.h"
#include <Actor/Cheese.h>
#include <Actor/Player.h>
#include <Actor/Mouse.h>
#include <Actor/Cat.h>
#include <Util/Util.h>

#include <Algorithm/GridExpander.h>

Stage1::Stage1()
{
	
}

void Stage1::OnInitialized()
{
	StageLevel::OnInitialized();

	grid = Util::LoadMapAsGrid("../Assets/Stage1.txt");
	
	collisionGrid = GridExpander::MakeAStarGrid(grid, Vector2(11, 6));

	SpawnActor<Player>();
	SpawnActor<Cheese>(cheesePos, Craft::Color::Yellow);
	SpawnActor<Mouse>(mousePos, Craft::Color::White);
	SpawnActor<Cat>(catPos1, Craft::Color::White);
	SpawnActor<Cat>(catPos2, Craft::Color::White);
}

void Stage1::Tick(float deltaTime)
{
	StageLevel::Tick(deltaTime);

	if (isStageCleared)
	{
		if (Input::Get().GetKeyDown(VK_LBUTTON))
		{
			ChangeStage(State::Stage2);
		}
	}
}
