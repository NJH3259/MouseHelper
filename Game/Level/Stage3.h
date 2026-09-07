#pragma once

#include <Level/StageLevel.h>
#include <Math/Vector2.h>

class Stage3 : public StageLevel
{
public:
	Stage3();

private:
	virtual void OnInitialized() override;

	virtual void Tick(float deltaTime) override;

private:
	Craft::Vector2 cheesePos = Craft::Vector2(200, 60);
	Craft::Vector2 mousePos = Craft::Vector2(20, 5);
	Craft::Vector2 catPos1 = Craft::Vector2(27, 61);
	Craft::Vector2 catPos2 = Craft::Vector2(200, 2);
	Craft::Vector2 catPos3 = Craft::Vector2(100, 33);
};
