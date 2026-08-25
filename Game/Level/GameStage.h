#pragma once

#include <Level/Level.h>
#include <Actor/Actor.h>
#include <Actor/Mouse.h>
#include <Actor/Cheese.h>

class GameStage : public Craft::Level
{

public:
	//특정 액터 탐색 함수
	//탬플릿으로 필요에 따라 여러 타입을 받을 수 있고 Actor 하위 클래스만 가능
	//typestring은 어떤 타입의 엑터를 가져올지 정하도록 하드 코딩으로 지정 -> 본 프로젝트에서 탐색 목표가 2 개이기 때문
	template<typename T, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
	std::shared_ptr<T> FindActor(std::string typeString)
	{
		std::shared_ptr<T> result;
		//FindActorInLevelWithType은 Actor타입을 반환하므로 커스텀 RTTI의 Cast를 통해 동적 형변환 진행(다운 캐스팅)
		result = Cast<T>(FindActorInLevelWithType(typeString));

		if (!result)
		{
			return nullptr;
		}

		return result;
	}

private:
	//레벨 내에서 쥐 액터가 치즈를 탐색하거나 고양이 액터가 쥐 액터를 탐색하도록 하기 위한 함수
	std::shared_ptr<Craft::Actor> FindActorInLevelWithType(std::string typeName)
	{
		if (typeName == "Cheese")
		{
			for (const auto& actor : actorList) {
				if (actor->IsTypeOf<Cheese>())
				{
					return actor;
				}
			}
		}

		else if (typeName == "Mouse")
		{
			for (const auto& actor : actorList) {
				if (actor->IsTypeOf<Mouse>())
				{
					return actor;
				}
			}
		}

		else {
			return nullptr;
		}
	}
};

