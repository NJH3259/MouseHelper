#include "Player.h"
#include <Render/Renderer.h>
#include <string>

using namespace Craft;

void Player::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	//플레이어 액터는 보이지 않지만 위치는 마우스 위치임
	//position.x = Input::Get().GetMousePosition().x;
	//position.y = Input::Get().GetMousePosition().y;
	position = Input::Get().GetMousePosition();

	if (Input::Get().GetKeyDown(VK_LBUTTON))
	{
		//현재 마우스 커서 위치에 고양이 액터가 있는지 확인
		//고양이 액터가 있으면
		if (CheckCatOnPosition())
		{
			//플레이어 액터의 고양이 집은 상태 true
			//고양이 액터의 Mouse 추적 해제
		}


	}

	if (Input::Get().GetKey(VK_LBUTTON))
	{
		int mouseX = Input::Get().GetMousePosition().x;
		int mouseY = Input::Get().GetMousePosition().y;

		std::string mousePosition = "(" + std::to_string(mouseX) + ", " + std::to_string(mouseY) + ")";

		Renderer::GetRenderer().Submit(mousePosition, Vector2(50, 20), Color::White, 5);

		//고양이 액터를 집은 상태라면 고양이 액터 위치를 마우스 커서 위치로 지정
	}

	if (Input::Get().GetKeyUp(VK_LBUTTON))
	{
		if(!isHoldingCat)
		{
			return;
		}

		//고양이 액터를 집은 상태라면
		
		//현재 마우스 위치를 집은 고양이 액터 위치로 지정
		Vector2 position = Input::Get().GetMousePosition();

		//고양이 액터의 쥐 추적 재활성화

		//플레이어 액터의 고양이 집은 상태 false
	}

	//std::string actorPosition = "(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")";
	//Renderer::GetRenderer().Submit(actorPosition, Vector2(20, 20), Color::White, 5);
}

bool Player::CheckCatOnPosition()
{
	//레벨의 액터 리스트에서 현재 마우스 위치와 겹치는 액터가 있는지 검사
	//있으면 currentCat에 해당 액터 주소 대입 및 return true
	return false;
}
