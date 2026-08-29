#pragma once

#include <Actor/Actor.h>
#include <Actor/Cat.h>
#include <Input/Input.h>

class Player : public Craft::Actor
{
public:
	Player()
	{

	}

private:
	virtual void Tick(float deltaTime) override;

	bool CheckCatOnPosition();

private:
	bool isHoldingCat = false;

	std::shared_ptr<Cat> currentCat = nullptr;
};

