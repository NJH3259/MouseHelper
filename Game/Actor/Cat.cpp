#include "Cat.h"
#include <Level/Level.h>
#include <Actor/Mouse.h>
#include <Input/Input.h>
#include <Util/Util.h>

#include<cassert>

Cat::Cat(const Craft::Vector2 position, Craft::Color color)
	:Actor(" ", position, color)
{
	image = Util::LoadImageFromFile("Cat", "../Assets/");

	sortingOrder = 1;

	mouse = FindMouseInLevel();
}

void Cat::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	IsActorStoped();

	if(!isHolded)
	{
		MoveToMouse();
	}
	else
	{
		position = GetMousePosition();
	}
}

void Cat::Draw()
{}

void Cat::OnCollision(const std::shared_ptr<Actor>&other)
{
	//게임 정지
}

//A* 알고리즘으로 쥐를 향해 이동하는 함수
void Cat::MoveToMouse()
{
	assert(mouse && "mouse should not be null");

	//플레이어에게 붙잡힌 상태가 아니라면

	//탐색한 mouse를 향해 A*알고리즘으로 경로 탐색

	//mouse를 향해 최적 경로로 한칸 이동
}

//마우스 커서 위치를 받아서 고양이 위치를 이동시킬 함수
Craft::Vector2 Cat::GetMousePosition()
{
	return Craft::Input::Get().GetMousePosition();
}

std::shared_ptr<Mouse> Cat::FindMouseInLevel()
{
	std::shared_ptr<Mouse> mouse = nullptr;
	
	//Level의 ActorList에서 Cheese 탐색
	mouse = GetOwner()->FindActor<Mouse>();

	assert(mouse && "mouse should not be null");

	return mouse;
}
