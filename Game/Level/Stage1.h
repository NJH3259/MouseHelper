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

	virtual void Tick(float deltaTime) override;

private:
	Craft::Vector2 position = Craft::Vector2(20, 20);
	Craft::Vector2 catPos1 = Craft::Vector2(40, 20);
	Craft::Vector2 catPos2 = Craft::Vector2(60, 40);

	std::vector<std::vector<int>> grid;
};

