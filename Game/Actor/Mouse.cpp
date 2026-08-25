#include "Mouse.h"
#include <Level/Level.h>
#include <Util/Util.h>

#include <cassert>

using namespace Craft;

Mouse::Mouse(const Vector2 position, Color color)
	:Actor(" ", position, color)
{
	image = Util::LoadImageFromFile("Mouse.txt", "../Assets/");

	//현재 레벨 객체의 FindActorInLevelWithType함수 사용

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
void Mouse::MoveToCheese()
{
	//레벨 내 액터 중 목표(치즈)를 탐색한다. -> 목표는 움직이지 않으므로 처음 위치를 저장해두고 사용하기

	//탐색에 성공하면 치즈를 향해 A* 알고리즘으로 최적 경로를 탐색한다.

	//최적 경로를 따라 이동한다.
}

std::shared_ptr<Cheese> Mouse::FindCheeseInLevel()
{
	std::shared_ptr<Cheese> cheese = nullptr;

	//Level의 ActorList에서 Cheese 탐색

	assert(cheese && "Cheese should not be null");
	return std::shared_ptr<Cheese>();
}
