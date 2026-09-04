#pragma once

#include <Actor/Actor.h>

using namespace Craft;
class Cheese : public Craft::Actor
{
	TYPE_DECLARATIONS(Cheese, Actor)

public:
	Cheese(const Vector2 position, Color color);

	inline Vector2 GetPivot() { return pivot; }

private:
	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;

	Vector2 pivot = (0, 0);
};