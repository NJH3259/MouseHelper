#pragma once

#include <Actor/Actor.h>
#include <Input/Input.h>

class Player : public Craft::Actor
{
public:
	Player()
	{

	}

private:
	virtual void Tick(float deltaTime) override;

private:
};

