#pragma once

#include <Level/Level.h>
#include <Math/Vector2.h>

class Stage1 : public Craft::Level
{
private:
	virtual void OnInitialized() override;

private:
	Craft::Vector2 position = Craft::Vector2(20, 20);
	Craft::Vector2 catPos = Craft::Vector2(40, 20);
};

