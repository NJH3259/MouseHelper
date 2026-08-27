#include "Player.h"
#include <Level/Level.h>
#include <Render/Renderer.h>

#include <cassert>
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
			assert(currentCat && "currentCat should not be null in here");
			
			//플레이어 액터의 고양이 집은 상태 true
			isHoldingCat = true;
			
			//고양이 Actor가 잡힌 상태를 true로 변경 -> 고양이 액터 내부 Tick에서 플래그에 따라 로직 변화(플레이어 마우스 따라가도록 변경)
			currentCat->SetIsHolded();
		}
	}

	if (Input::Get().GetKey(VK_LBUTTON))
	{
		//-----------------------------------------------------testcode------------------------------------//
		int mouseX = Input::Get().GetMousePosition().x;
		int mouseY = Input::Get().GetMousePosition().y;
		
		std::string mousePosition = "(" + std::to_string(mouseX) + ", " + std::to_string(mouseY) + ")";

		Renderer::GetRenderer().Submit(mousePosition, Vector2(50, 20), Color::White, 5);
		//-------------------------------------------------------------------------------------------------//
		// 
		//고양이 액터를 집은 상태라면 고양이 액터 위치를 마우스 커서 위치로 지정 -> Cat의 Tick에서 플래그에 따라 따라가도록 하고 있음
	}

	if (Input::Get().GetKeyUp(VK_LBUTTON))
	{
		if(!isHoldingCat)
		{
			return;
		}

		//고양이 액터를 집은 상태라면

		//고양이 액터 내부의 잡힘 상태 해제
		if(currentCat)
		{
			currentCat->SetIsHolded();
		}

		//현재 마우스 위치를 집은 고양이 액터 위치로 지정 -> 고양이의 Tick()에서 실행 중

		//고양이 액터의 쥐 추적 재활성화 -> 고양이의 Tick()에서 실행 중

		//플레이어 액터의 고양이 집은 상태 해제
		isHoldingCat = false;
	}

	//-------------------------------------------------test code----------------------------------------------//
	//std::string actorPosition = "(" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")";
	//Renderer::GetRenderer().Submit(actorPosition, Vector2(20, 20), Color::White, 5);

	//std::string isHoldingCatString = std::to_string(isHoldingCat);
	//Renderer::GetRenderer().Submit(isHoldingCatString, Vector2(20, 20), Color::White, 5);
	// 
	//-------------------------------------------------test code----------------------------------------------//
}

//레벨의 액터 리스트에서 현재 마우스 위치와 겹치는 액터가 있는지 검사
bool Player::CheckCatOnPosition()
{
	std::vector<std::shared_ptr<Cat>> catList;
	
	//현재 레벨의 모든 Cat을 vector로 가져옴
	catList = GetOwner()->FindSpecificActors<Cat>();

	//가져온 vector의 모든 Cat에 대해
	for (const auto& cat : catList)
	{
		//Cat의 position 범위(position ~ position+size)내에 있으면
		if (cat->GetPosition() == position)
		{
			//currentCat에 해당 Cat 대입
			currentCat = cat;
			return true;
		}
	}

	//없으면 false 반환
	return false;
}
