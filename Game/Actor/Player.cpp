#include "Player.h"
#include <Render/Renderer.h>
#include <string>

using namespace Craft;

void Player::Tick(float deltaTime)
{
	if (Input::Get().GetKey(VK_LBUTTON))
	{
		int mouseX = Input::Get().GetMousePosition().x;
		int mouseY = Input::Get().GetMousePosition().y;

		std::string mousePosition = "(" + std::to_string(mouseX) + ", " + std::to_string(mouseY) + ")";

		Renderer::GetRenderer().Submit(mousePosition, Vector2(50, 20), Color::White, 5);
	}
}
