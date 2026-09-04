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
}

void Mouse::BeginPlay()
{
	Actor::BeginPlay();

	if(!cheese)
	{
		//현재 레벨 객체의 FindActorInLevelWithType함수 사용
		cheese = FindCheeseInLevel();
	}
}

void Mouse::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

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
}

void Mouse::OnCollision(const std::shared_ptr<Actor>&other)
{
	//충돌한 액터 타입이 Cat인 경우
	if (Cast<Cat>(other))
	{
		GetOwner()->SetIsLevelStoped(true);
		//todo: 레벨 실패 처리
	}
	//충돌한 액터 타입이 Cheese인 경우
	else if (Cast<Cheese>(other))
	{
		GetOwner()->SetIsLevelStoped(true);
		//todo: 레벨 클리어 처리
	}
}

//todo: 구현 필요
void Mouse::MoveToCheese()
{
	assert(cheese && "cheese should not be null");

	//치즈를 향해 A* 알고리즘으로 최적 경로를 탐색한다.

	//최적 경로를 따라 이동한다.
}

std::shared_ptr<Cheese> Mouse::FindCheeseInLevel()
{
	std::shared_ptr<Cheese> cheese = nullptr;

	//Level의 ActorList에서 Cheese 탐색
	cheese = GetOwner()->FindActor<Cheese>();

	assert(cheese && "Cheese should not be null");
	return cheese;
}
