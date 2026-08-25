#pragma once

#include <Actor/Actor.h>
#include <Math/Vector2.h>
#include <Math/Color.h>

#include <string>
#include <memory>

class Cheese;

class Mouse : public Craft::Actor
{
	TYPE_DECLARATIONS(Mouse, Actor)

public:
	Mouse(const Craft::Vector2 position, Craft::Color color);
	
private:
	virtual void Tick(float deltaTime) override;

	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

	void MoveToCheese();

	std::shared_ptr<Cheese> FindCheeseInLevel();

private:
	Craft::Vector2 position = Craft::Vector2(0, 0);

	std::string image;

	bool isGameStoped = false;

	std::shared_ptr<Cheese> cheese = nullptr;
};

