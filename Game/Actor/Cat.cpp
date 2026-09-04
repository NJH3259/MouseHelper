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

	moveTimer.SetTargetTime(0.18f);

	sortingOrder = 1;

	isActorStoped = false;

	pivot = Vector2(position.x + (int)(GetWidth() / 2), position.y + (int)(GetHeight() / 2));
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

	moveTimer.Tick(deltaTime);

	pivot = Vector2(position.x + (int)(GetWidth() / 2), position.y + (int)(GetHeight() / 2));


	if(!isHolded)
	{
		isSetOffset = false;
		curOffset = Vector2(0, 0);
		
		//todo: 마우스 클릭을 때서 내려놓은 곳에 벽이 있는 경우 (그리드와 피봇이 겹치는 경우)

		//레벨에 position을 받아 해당 위치에 벽이 있는지 반환하는 함수 구현하기

		//반환값을 보고 현재 위치에 벽이 있다면

		//방법 1. 가장 가까운, 벽과 겹치지 않는 위치로 이동시킨다.
		//방법 2. 위치를 초기화 시킨다. -> 방법 2는 게임 난이도 조절에 악영향을 미칠 수 있음

		//이 연산을 Tick에서 매번 수행하면 혹시나 벽과 겹치는 상황을 없앨 수 있지만, 매 Tick마다 연산을 진행하여 성능이 떨어질 수 있음 -> 고민해볼 문제

		if(moveTimer.IsTimeOut())
		{
			MoveToMouse();
			moveTimer.Reset();
		}
	}
	else
	{
		if (!isSetOffset)
		{
			curOffset = CalculateOffset(GetMousePosition());
			isSetOffset = true;
		}

		position = GetMousePosition() - curOffset;
	}

	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
	if (std::dynamic_pointer_cast<StageLevel>(GetOwner())->GetDebugMod())
	{
		Renderer::GetRenderer().Submit(" ", pivot, Color::B_Red, 5);

		if (isHolded)
		{
			std::string currentPos = "잡힌 고양이의 위치: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")";
			Renderer::GetRenderer().Submit(currentPos, Vector2(2, 4), Color::White, 5);
			std::string pivotPos = "액터의 피봇 위치: (" + std::to_string(pivot.x) + ", " + std::to_string(pivot.y) + ")";
			Renderer::GetRenderer().Submit(pivotPos, Vector2(2, 5), Color::White, 5);
		}
	}
	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
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

	if (!isHolded)
	{
		path.clear();
		std::vector<std::vector<int>> grid = std::dynamic_pointer_cast<StageLevel>(GetOwner())->GetGridForPath();
		path = catPathFinder.FindPath(pivot, mouse->GetPivot(), grid);

		//디버그 모드인 경우 경로 그리기
		if (std::dynamic_pointer_cast<StageLevel>(GetOwner())->GetDebugMod())
		{
			catPathFinder.DisplayPath(grid, path);
		}

		//위치를 찾은 경로의 다음 픽셀로 변경
		if (path.size() > 1)
		{
			position = path[1] - Vector2((int)(GetWidth() / 2), (int)(GetHeight() / 2));
		}
	}
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
