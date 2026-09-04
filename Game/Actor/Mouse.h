#pragma once

#include <Actor/Actor.h>

#include <memory>

class Cheese;

class Mouse : public Craft::Actor
{
	TYPE_DECLARATIONS(Mouse, Actor)

public:
	Mouse(const Craft::Vector2 position, Craft::Color color);

	Craft::Vector2 GetPivot() { return pivot; }
	
private:
	virtual void BeginPlay() override;
	
	virtual void Tick(float deltaTime) override;

	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

	// A* 알고리즘으로 목표를 향한 최적 루트를 찾고 해당 루트를 따라 1픽셀 이동시키는 함수
	void MoveToCheese();

	// A* 알고리즘에 사용할 목표물을 가져오는 함수
	std::shared_ptr<Cheese> FindCheeseInLevel();

private:
	std::shared_ptr<Cheese> cheese = nullptr;

	Craft::Vector2 pivot = (0, 0);
};

