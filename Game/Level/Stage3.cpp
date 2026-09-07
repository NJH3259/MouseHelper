#include "Stage3.h"
#include <Actor/Cheese.h>
#include <Actor/Player.h>
#include <Actor/Mouse.h>
#include <Actor/Cat.h>
#include <Util/Util.h>

#include <Algorithm/GridExpander.h>

Stage3::Stage3()
{

}

void Stage3::OnInitialized()
{
	StageLevel::OnInitialized();

	grid = Util::LoadMapAsGrid("../Assets/Stage3.txt");

	collisionGrid = GridExpander::MakeAStarGrid(grid, Vector2(11, 6));

	mouseManipulationCount = 10;

	SpawnActor<Player>();
	SpawnActor<Cheese>(cheesePos, Craft::Color::Yellow);
	SpawnActor<Mouse>(mousePos, Craft::Color::White);
	SpawnActor<Cat>(catPos1, Craft::Color::White);
	SpawnActor<Cat>(catPos2, Craft::Color::White);
	SpawnActor<Cat>(catPos3, Craft::Color::White);
}

void Stage3::Tick(float deltaTime)
{
	StageLevel::Tick(deltaTime);

	if (isStageCleared)
	{
		if (Input::Get().GetKeyDown(VK_LBUTTON))
		{
			ChangeStage(State::Stage4);
		}
	}
}