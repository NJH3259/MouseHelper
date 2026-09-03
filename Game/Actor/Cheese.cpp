#include "Cheese.h"
#include <Util/Util.h>
#include <Actor/Mouse.h>

Cheese::Cheese(const Vector2 position, Color color)
	:Actor(" ", position, color)
{
	image = Util::LoadImageFromFile("Cheese.txt", "../Assets/");

	sortingOrder = 1;
}

void Cheese::OnCollision(const std::shared_ptr<Actor>&other)
{
	if (Craft::Cast<Mouse>(other))
	{

	}
}
