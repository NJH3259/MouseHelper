#pragma once

#include <Level/Level.h>
#include <Math/Vector2.h>

class Stage1 : public Craft::Level
{
private:
	virtual void OnInitialized() override;

private:
	Craft::Vector2 position = (10, 10);
};

