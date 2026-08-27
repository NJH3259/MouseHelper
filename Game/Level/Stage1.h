#pragma once

#include <Level/StageLevel.h>
#include <Math/Vector2.h>

class Stage1 : public StageLevel
{
private:
	virtual void OnInitialized() override;

private:
	Craft::Vector2 position = (10, 10);
};

