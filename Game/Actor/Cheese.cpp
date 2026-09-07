#include "Cheese.h"
#include <Level/StageLevel.h>
#include <Util/Util.h>
#include <Actor/Mouse.h>

Cheese::Cheese(const Vector2 position, Color color)
	:Actor(" ", position, color)
{
	ChangeImage(Util::LoadImageFromFile("Cheese.txt", "../Assets/"));

	sortingOrder = 1;

	pivot = Vector2(position.x + (int)(GetWidth() / 2), position.y + (int)(GetHeight() / 2));
}

void Cheese::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	if (ISDEBUGMOD)
	{
		// 피봇 위치 화면에 표시
		Renderer::GetRenderer().Submit(" ", pivot, Color::B_Yellow, 7);
	}
}

void Cheese::OnCollision(const std::shared_ptr<Actor>&other)
{
	if (Craft::Cast<Mouse>(other))
	{

	}
}
