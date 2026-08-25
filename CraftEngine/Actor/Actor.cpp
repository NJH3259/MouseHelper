#include <Engine/Engine.h>
#include "Actor.h"
#include <Render/Renderer.h>

namespace Craft {
	Actor::Actor(const std::string& image, const Vector2& position, Color color)
		: image(image), position(position), color(color), width(static_cast<int>(image.length()))
	{}

	Actor::~Actor()
	{}

	void Actor::BeginPlay()
	{
		hasBeganPlay = true;
	}

	void Actor::Tick(float deltaTime)
	{}

	void Actor::Draw()
	{
		if (!IsActive()) {
			return;
		}

		//렌더러에 필요한 데이터 제출
		Renderer::GetRenderer().Submit(image, position, color, sortingOrder);
	}

	void Actor::OnCollision(const std::shared_ptr<Actor>& other)
	{}

	void Actor::Destroy()
	{
		//삭제 예약
		hasExpired = true;
	}
	void Actor::QuitGame()
	{
		//엔진 종료 요청
		Engine::Get().Quit(); //static선언으로 엔진 객체 접근 가능
	}

	void Actor::SetPosition(const Vector2& newPosition) {
		if (position == newPosition) {
			return;
		}

		position = newPosition;
	}

	Vector2 Actor::GetScreenSize() const
	{
		return Vector2(Engine::Get().GetWidth(), Engine::Get().GetHeight());
	}
	
}