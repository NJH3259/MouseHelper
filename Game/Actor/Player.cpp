#include "Player.h"
#include <Render/Renderer.h>
#include <string>

using namespace Craft;

void Player::Tick(float deltaTime)
{
	if (Input::Get().GetKeyDown(VK_LBUTTON))
	{
		//현재 마우스 커서 위치에 고양이 액터가 있는지 확인

		//고양이 액터가 있으면
		//플레이어 액터의 고양이 집은 상태 true
		//고양이 액터의 Mouse 추적 해제


	}

	if (Input::Get().GetKey(VK_LBUTTON))
	{
		int mouseX = Input::Get().GetMousePosition().x;
		int mouseY = Input::Get().GetMousePosition().y;

		std::string mousePosition = "(" + std::to_string(mouseX) + ", " + std::to_string(mouseY) + ") █ ░ ▒ ▓";

		Renderer::GetRenderer().Submit(mousePosition, Vector2(50, 20), Color::White, 5);

		//고양이 액터를 집은 상태라면 고양이 액터 위치를 마우스 커서 위치로 지정
	}

	if (Input::Get().GetKeyUp(VK_LBUTTON))
	{
		//고양이 액터를 집은 상태라면

		//현재 마우스 위치를 집은 고양이 액터 위치로 지정

		//고양이 액터의 쥐 추적 재활성화

		//플레이어 액터의 고양이 집은 상태 false
	}
}
