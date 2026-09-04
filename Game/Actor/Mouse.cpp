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

	pivot = Vector2(position.x + (int)(GetWidth() / 2), position.y + (int)(GetHeight() / 2));

	moveTimer.SetTargetTime(0.3f);
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
}

void Mouse::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	moveTimer.Tick(deltaTime);

	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
	if (std::dynamic_pointer_cast<StageLevel>(GetOwner())->GetDebugMod())
	{
		Renderer::GetRenderer().Submit(" ", pivot, Color::B_Green, 7);
	}
	//-----------------------------------------------------------Debug Mod--------------------------------------------------------/

	//게임 스테이지 패배 및 승리 시 Actor들 멈춤
	IsActorStoped();

	pivot = Vector2(position.x + (int)(GetWidth() / 2), position.y + (int)(GetHeight() / 2));

	//매 프레임마다 최적 A* 경로를 탐색해서 목표를 향해 이동
	MoveToCheese();

	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
	if (std::dynamic_pointer_cast<StageLevel>(GetOwner())->GetDebugMod())
	{
		Renderer::GetRenderer().Submit(" ", pivot, Color::B_Green, 5);
	}
	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
}

void Mouse::OnCollision(const std::shared_ptr<Actor>&other)
{
	//충돌한 액터 타입이 Cheese인 경우
	if (Cast<Cheese>(other))
	{
		GetOwner()->SetIsLevelStoped(true);
		//todo: 레벨 클리어 처리
	}
}

//todo: 구현 필요
void Mouse::MoveToCheese()
{
	assert(cheese && "cheese should not be null");

	//탐색한 mouse를 향해 A*알고리즘으로 경로 탐색
	path.clear();
	path = mousePathFinder.FindPath(pivot, cheese->GetPivot(), gridForPath);

	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
	if (ISDEBUGMOD)
	{
		//디버그 모드인 경우 경로 그리기
		mousePathFinder.DisplayPath(gridForPath, path, Color::B_Blue);
	}
	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//

	// 매 프레임마다 이동은 지나치게 빠르므로 이동은 제한 시간을 두고 이동한다.
	if (moveTimer.IsTimeOut())
	{
		// 피봇이 mouse의 피봇과 완전히 겹치는 경우 path의 size는 1이다(자기 자신의 위치만 들어있음)
		if (path.size() > 1)
		{
			// mouse를 향해 최적 경로로 한칸 이동
			position = path[1] - Vector2((int)(GetWidth() / 2), (int)(GetHeight() / 2));
		}
		moveTimer.Reset();
	}
}

std::shared_ptr<Cheese> Mouse::FindCheeseInLevel()
{
	std::shared_ptr<Cheese> cheese = nullptr;

	//Level의 ActorList에서 Cheese 탐색
	cheese = GetOwner()->FindActor<Cheese>();

	assert(cheese && "Cheese should not be null");
	return cheese;
}
