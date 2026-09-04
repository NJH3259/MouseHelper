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

	void MoveToCheese();

	std::shared_ptr<Cheese> FindCheeseInLevel();

private:
	std::shared_ptr<Cheese> cheese = nullptr;

	Craft::Vector2 pivot = (0, 0);
};

