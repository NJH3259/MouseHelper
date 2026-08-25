#include "Cat.h"
#include <Level/GameStage.h>
#include <Util/Util.h>

#include<cassert>

Cat::Cat(const Craft::Vector2 position, Craft::Color color)
	:Actor(" ", position, color)
{
	image = Util::LoadImageFromFile("Cat", "../Assets/");

	mouse = FindMouseInLevel();
}

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
{
	assert(mouse && "mouse should not be null");

	//탐색한 mouse를 향해 A*알고리즘으로 경로 탐색

	//mouse를 향해 최적 경로로 한칸 이동
}

std::shared_ptr<Mouse> Cat::FindMouseInLevel()
{
	std::shared_ptr<Mouse> mouse = nullptr;
	
	//Level의 ActorList에서 Cheese 탐색
	mouse = Cast<GameStage>(GetOwner())->FindActor<Mouse>("Mouse");
	
	assert(mouse && "Cheese should not be null");

	return std::shared_ptr<Mouse>();
}
