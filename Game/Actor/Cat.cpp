#include "Cat.h"
#include <Level/StageLevel.h>
#include <Actor/Mouse.h>
#include <Input/Input.h>
#include <Util/Util.h>
#include <Render/Renderer.h>

#include<cassert>
#include <memory>

using namespace Craft;

Cat::Cat(const Vector2 position, Color color)
	:Actor(" ", position, color)
{
	ChangeImage(Util::LoadImageFromFile("Cat.txt", "../Assets/"));

	sortingOrder = 1;

	isActorStoped = false;
}

void Cat::BeginPlay()
{
	if (!mouse) {
		mouse = FindMouseInLevel();
	}
}

void Cat::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	IsActorStoped();

	Vector2 pivot = Vector2(position.x + (int)(GetWidth() / 2), position.y + (int)(GetHeight() / 2));


	if(!isHolded)
	{
		isSetOffset = false;
		MoveToMouse();
	}
	else
	{
		if (!isSetOffset)
		{
			curOffset = CalculateOffset(GetMousePosition());
			isSetOffset = true;
		}

		position = GetMousePosition() - curOffset;

		if (std::dynamic_pointer_cast<StageLevel>(GetOwner())->GetDebugMod())
		{
			std::string currentPos = "잡힌 고양이의 위치: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")";
			Renderer::GetRenderer().Submit(currentPos, Vector2(2, 4), Color::White, 5);
			std::string pivotPos = "액터의 피봇 위치: (" + std::to_string(pivot.x) + ", " + std::to_string(pivot.y) + ")";
			Renderer::GetRenderer().Submit(pivotPos, Vector2(2, 5), Color::White, 5);
		}
	}
}

void Cat::OnCollision(const std::shared_ptr<Actor>&other)
{

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
	return Input::Get().GetMousePosition();
}

std::shared_ptr<Mouse> Cat::FindMouseInLevel()
{
	std::shared_ptr<Mouse> mouse = nullptr;
	
	//Level의 ActorList에서 Mouse 탐색
	mouse = GetOwner()->FindActor<Mouse>();

	assert(mouse && "mouse should not be null");

	return mouse;
}

//고양이가 플레이어에게 잡힌 경우 마우스 커서를 따라가도록 함
Vector2 Cat::CalculateOffset(Vector2 mousePosition)
{

	int offsetX = mousePosition.x - position.x;
	int offsetY = mousePosition.y - position.y;

	Vector2 offset = Vector2(offsetX, offsetY);

	return offset;
}
