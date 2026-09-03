#pragma once

#include <Actor/Actor.h>

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

	void MoveToMouse();

	Craft::Vector2 GetMousePosition();

	std::shared_ptr<Mouse> FindMouseInLevel();

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
};

