#include "Stage2.h"
#include <Actor/Cheese.h>
#include <Actor/Player.h>
#include <Actor/Mouse.h>
#include <Actor/Cat.h>
#include <Util/Util.h>

#include <Algorithm/GridExpander.h>

Stage2::Stage2()
{

}

void Stage2::OnInitialized()
{
	StageLevel::OnInitialized();

	grid = Util::LoadMapAsGrid("../Assets/Stage2.txt");

	collisionGrid = GridExpander::MakeAStarGrid(grid, Vector2(11, 6));

	SpawnActor<Player>();
	SpawnActor<Cheese>(cheesePos, Craft::Color::Yellow);
	SpawnActor<Mouse>(mousePos, Craft::Color::White);
	SpawnActor<Cat>(catPos1, Craft::Color::White);
	SpawnActor<Cat>(catPos2, Craft::Color::White);
	SpawnActor<Cat>(catPos3, Craft::Color::White);
	SpawnActor<Cat>(catPos4, Craft::Color::White);
}

void Stage2::Tick(float deltaTime)
{
	StageLevel::Tick(deltaTime);

	if (isStageCleared)
	{
		if (Input::Get().GetKeyDown(VK_LBUTTON))
		{
			ChangeStage(State::Stage3);
		}
	}
}