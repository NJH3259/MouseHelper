#pragma once

#include <Level/StageLevel.h>
#include <Math/Vector2.h>

class Stage1 : public StageLevel
{
public:
	Stage1();

private:
	virtual void OnInitialized() override;

	virtual void ResetLevel() override;

private:
	Craft::Vector2 cheesePos = Craft::Vector2(200, 40);
	Craft::Vector2 mousePos = Craft::Vector2(20, 20);
	Craft::Vector2 catPos1 = Craft::Vector2(150, 10);
	Craft::Vector2 catPos2 = Craft::Vector2(150, 50);
};

