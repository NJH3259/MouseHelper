#pragma once

#include <Level/StageLevel.h>
#include <Math/Vector2.h>

class Stage2 : public StageLevel
{
public:
	Stage2();

private:
	virtual void OnInitialized() override;

	virtual void Tick(float deltaTime) override;

private:
	Craft::Vector2 cheesePos = Craft::Vector2(200, 30);
	Craft::Vector2 mousePos = Craft::Vector2(20, 30);
	Craft::Vector2 catPos1 = Craft::Vector2(80, 7);
	Craft::Vector2 catPos2 = Craft::Vector2(120, 7);
	Craft::Vector2 catPos3 = Craft::Vector2(80, 57);
	Craft::Vector2 catPos4 = Craft::Vector2(120, 57);
};