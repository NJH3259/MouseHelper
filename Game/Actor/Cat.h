#pragma once

#include <Actor/Actor.h>
#include <Algorithm/AStar.h>
#include <Util/Timer.h>

#include <memory>

class Mouse;

class Cat : public Craft::Actor
{
	TYPE_DECLARATIONS(Cat, Actor)

public:
	Cat(const Craft::Vector2 position, Craft::Color color);

	inline void SetIsHoldedState() { isHolded = true; }
	inline void ReleaseIsHoldedState() { isHolded = false; }

private:
	virtual void BeginPlay() override;

	virtual void Tick(float deltaTime) override;

	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

	// A*로 최적의 경로 탐색 및 목표를 향해 이동하는 함수
	void MoveToMouse();

	// 위치 이동을 위해 마우스 커서 위치를 받아오는 함수
	Craft::Vector2 GetMousePosition();

	// 레벨 내에서 AStar 목적지로 설정할 Mouse 액터를 탐색하기 위한 함수
	std::shared_ptr<Mouse> FindMouseInLevel();

	// 마우스를 따라 이동할 때, 액터 생성 위치가 아닌 마우스에 잡힌 위치를 기준으로 이동하는 것 처럼 보이게 하기 위한 offset을 계산하는 함수
	Craft::Vector2 CalculateOffset(Craft::Vector2 mousePosition);

private:
	std::shared_ptr<Mouse> mouse = nullptr;

	//플레이어에게 잡힌 상태인지 판단을 위한 플래그
	bool isHolded = false;

	//플레이어에게 잡힌 경우 오프셋 계산을 한 번만 진행하기 위한 플래그
	bool isSetOffset = false;

	Craft::Vector2 curOffset = (0, 0);

	//피봇: A*로 경로 탐색을 할 때 중심이 되는 시작 위치
	Craft::Vector2 pivot = (0, 0);

	std::vector<std::vector<int>> gridForPath = {};

	AStar catPathFinder;
	std::vector<Vector2> path = {};

	Timer moveTimer;
};

