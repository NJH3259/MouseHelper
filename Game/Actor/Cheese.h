#pragma once

#include <Actor/Actor.h>

using namespace Craft;
class Cheese : public Actor
{
	TYPE_DECLARATIONS(Cheese, Actor)

public:
	Cheese(const Vector2 position, Color color);

private:
	virtual void Tick(float deltaTime) override;

	virtual void Draw() override;

	virtual void OnCollision(const std::shared_ptr<Actor>& other) override;
};

//█ ░ ▒ ▓ -> 음영에 그릴 문자 (3번째 거만 출력됨)

