#pragma once

#include <Level/Level.h>
#include <Actor/Actor.h>
#include <Actor/Mouse.h>
#include <Actor/Cheese.h>

class GameStage :public Craft::Level
{

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

