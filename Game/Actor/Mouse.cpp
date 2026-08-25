#include "Mouse.h"
#include <Level/Level.h>
#include <Util/Util.h>
#include <Level/GameStage.h>

#include <cassert>

using namespace Craft;

Mouse::Mouse(const Vector2 position, Color color)
	:Actor(" ", position, color)
{
	image = Util::LoadImageFromFile("Mouse.txt", "../Assets/");

	//현재 레벨 객체의 FindActorInLevelWithType함수 사용
	cheese = FindCheeseInLevel();

	ResetStopedState();
}

void Mouse::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	//게임 스테이지 패배 및 승리 시 Actor들 멈춤
	IsActorStoped();

	//매 프레임마다 최적 A* 경로를 탐색해서 목표를 향해 이동
	MoveToCheese();
}

void Mouse::OnCollision(const std::shared_ptr<Actor>&other)
{
	isActorStoped = true;
}

//todo: 구현 필요
//목표를 향해 A*알고리즘으로 길을 찾아가는 함수
void Mouse::MoveToCheese()
{
	assert(cheese && "cheese should not be null");

	//치즈를 향해 A* 알고리즘으로 최적 경로를 탐색한다.

	//최적 경로를 따라 이동한다.
}

//Actor를 소유한 레벨의 액터 목록에서 특정 타입의 액터를 가져와 반환
std::shared_ptr<Cheese> Mouse::FindCheeseInLevel()
{
	std::shared_ptr<Cheese> cheese = nullptr;

	//Level의 ActorList에서 Cheese 탐색
	cheese = Cast<GameStage>(GetOwner())->FindActor<Cheese>("Cheese");

	assert(cheese && "Cheese should not be null");
	return std::shared_ptr<Cheese>();
}
