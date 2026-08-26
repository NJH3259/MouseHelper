#pragma once

#include <Actor/Actor.h>

#include <memory>

class Mouse;

class Cat : public Craft::Actor
{
	TYPE_DECLARATIONS(Cat, Actor)

public:
	Cat(const Craft::Vector2 position, Craft::Color color);

private:
	virtual void Tick(float deltaTime) override;

	virtual void Draw() override;

	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

	void MoveToMouse();

	Craft::Vector2 GetMousePosition();

	std::shared_ptr<Mouse> FindMouseInLevel();

private:
	std::shared_ptr<Mouse> mouse = nullptr;

	//플레이어에게 잡힌 상태인지 판단을 위한 플래그
	bool isHolded = false;
};

