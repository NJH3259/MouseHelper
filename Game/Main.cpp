#include <GameManager/Game.h>
#include <Level/Stage1.h>

#include <iostream>

int main()
{
	Craft::Engine engine;
	engine.AddNewLevel<Stage1>();
	engine.Run();

	//Game game;
	//game.Run();

	//Ver.09060006
}	