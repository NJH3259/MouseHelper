#include "Cat.h"
#include <Level/GameStage.h>

#include<cassert>

Cat::Cat(const Craft::Vector2 position, Craft::Color color)
{}

void Cat::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	IsActorStoped();

	MoveToMouse();
}

void Cat::Draw()
{}

void Cat::OnCollision(const std::shared_ptr<Actor>&other)
{}

void Cat::MoveToMouse()
{}

std::shared_ptr<Mouse> Cat::FindMouseInLevel()
{
	std::shared_ptr<Mouse> mouse = nullptr;
	
	//Level의 ActorList에서 Cheese 탐색
	mouse = Cast<GameStage>(GetOwner())->FindActor<Mouse>("Cheese");
	
	assert(mouse && "Cheese should not be null");

	return std::shared_ptr<Mouse>();
}
