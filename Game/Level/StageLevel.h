#pragma once

#include <Level/Level.h>
#include <Input/Input.h>

using namespace Craft;
class StageLevel : public Level
{
protected:
	void Tick(float deltaTime)
	{
		Level::Tick(deltaTime);

		if (Input::Get().GetKeyDown(VK_ESCAPE))
		{
			isLevelStoped = !isLevelStoped;
		}
	}
};