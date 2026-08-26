#pragma once

#include "GameStage.h"
#include <Math/Vector2.h>

class Stage1 : public GameStage
{
private:
	virtual void OnInitialized() override;

private:
	Vector2 position = (10, 10);
};

