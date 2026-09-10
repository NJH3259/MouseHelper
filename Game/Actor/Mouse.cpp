#include "Mouse.h"
#include <Actor/Cheese.h>
#include <Actor/Cat.h>
#include <Level/StageLevel.h>
#include <Util/Util.h>
#include <Render/Renderer.h>

#include <memory>
#include <cassert>

using namespace Craft;

Mouse::Mouse(const Vector2 position, Color color)
	:Actor(" ", position, color)
{
	ChangeImage(Util::LoadImageFromFile("Mouse.txt", "../Assets/"));

	sortingOrder = 1;

	isActorStoped = false;

	currentPathIndex = 1;

	pivot = Vector2(position.x + (int)(GetWidth() / 2), position.y + (int)(GetHeight() / 2));

	moveTimer.SetTargetTime(0.2f);
}

void Mouse::BeginPlay()
{
	Actor::BeginPlay();

	if(!cheese)
	{
		//현재 레벨 객체의 FindActorInLevelWithType함수 사용
		cheese = FindCheeseInLevel();
	}

	if (gridForPath.empty())
	{
		gridForPath = std::dynamic_pointer_cast<StageLevel>(GetOwner())->GetGridForPath();
	}

	path.clear();
	path = mousePathFinder.FindPath(pivot, cheese->GetPivot(), gridForPath);
}

void Mouse::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	moveTimer.Tick(deltaTime);

	//게임 스테이지 패배 및 승리 시 Actor 멈춤
	if (isActorStoped)
	{
		return;
	}

	pivot = Vector2(position.x + (int)(GetWidth() / 2), position.y + (int)(GetHeight() / 2));

	//매 프레임마다 최적 A* 경로를 탐색해서 목표를 향해 이동
	MoveToCheese(currentPathIndex);

	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
	if (std::dynamic_pointer_cast<StageLevel>(GetOwner())->GetDebugMod())
	{
		Renderer::GetRenderer().Submit(" ", pivot, Color::B_Green, 7);
	}
	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
}

void Mouse::OnCollision(const std::shared_ptr<Actor>&other)
{
	//충돌한 액터 타입이 Cheese인 경우
	if (Cast<Cheese>(other))
	{
		// 레벨 클리어 처리
		std::dynamic_pointer_cast<StageLevel>(GetOwner())->SetIsStageCleared(true);
	}
}

// 치즈를 향해 경로를 탐색하고 1픽셀 단위로 이동함. 이동 후 다시 경로 탐색
void Mouse::MoveToCheese(int& index)
{
	assert(cheese && "cheese should not be null");

	if (index >= path.size())
	{
		return;
	}

	// 매 프레임마다 이동은 지나치게 빠르므로 이동은 제한 시간을 두고 이동한다.
	if (moveTimer.IsTimeOut())
	{
		// 처음 탐색한 경로를 기준으로 인덱스를 늘려가며 이동
		position = path[index] - Vector2((int)(GetWidth() / 2), (int)(GetHeight() / 2));

		index += 1;

		moveTimer.Reset();
	}

	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
	if (ISDEBUGMOD)
	{
		//디버그 모드인 경우 경로 그리기
		mousePathFinder.DisplayPath(gridForPath, path, Color::B_Blue, index);
	}
	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
}

std::shared_ptr<Cheese> Mouse::FindCheeseInLevel()
{
	std::shared_ptr<Cheese> cheese = nullptr;

	//Level의 ActorList에서 Cheese 탐색
	cheese = GetOwner()->FindActor<Cheese>();

	assert(cheese && "Cheese should not be null");
	return cheese;
}
