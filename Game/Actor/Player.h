#pragma once

#include <Actor/Actor.h>
#include <Actor/Cat.h>
#include <Input/Input.h>

class Craft::Vector2;

class Player : public Craft::Actor
{
public:
	Player()
	{

	}

private:
	virtual void Tick(float deltaTime) override;

	// 레벨의 액터 리스트에서 현재 마우스 커서의 위치와 겹치는 고양이 액터가 있는지 검사하는 함수
	bool CheckCatOnPosition();

	// 마우스 커서가 액터의 이미지 범위 내에 있는지 검사하는 함수
	bool CheckMouseInImage(Craft::Vector2 catPos, Craft::Vector2 catImageSize, Craft::Vector2 MousePosition);

private:
	bool isHoldingCat = false;

	std::shared_ptr<Cat> currentCat = nullptr;
};

