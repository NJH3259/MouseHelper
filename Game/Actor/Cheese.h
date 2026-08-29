#pragma once

#include <Actor/Actor.h>

using namespace Craft;
class Cheese : public Actor
{
	TYPE_DECLARATIONS(Cheese, Actor)

public:
	Cheese(const Vector2 position, Color color);

private:
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;
};