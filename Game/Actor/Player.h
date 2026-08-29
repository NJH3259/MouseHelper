#pragma once

#include <Actor/Actor.h>
#include <Actor/Cat.h>
#include <Input/Input.h>

class Craft::Vector2;

class Player : public Craft::Actor
{
public:
	Player()
	{

	}

private:
	virtual void Tick(float deltaTime) override;

	bool CheckCatOnPosition();

	bool CheckMouseInImage(Craft::Vector2 catPos, Craft::Vector2 catImageSize, Craft::Vector2 MousePosition);

private:
	bool isHoldingCat = false;

	std::shared_ptr<Cat> currentCat = nullptr;
};

