#pragma once

#include <Level/StageLevel.h>
#include <Math/Vector2.h>

class Stage1 : public StageLevel
{
public:
	Stage1();

private:
	virtual void OnInitialized() override;

	virtual void Tick(float deltaTime) override;

private:
	Craft::Vector2 cheesePos = Craft::Vector2(120, 55);
	Craft::Vector2 mousePos = Craft::Vector2(25, 5);
	Craft::Vector2 catPos1 = Craft::Vector2(150, 10);
	Craft::Vector2 catPos2 = Craft::Vector2(50, 50);
};

