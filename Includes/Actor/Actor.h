#pragma once
#include <memory> //std::weak_ptr사용을 위함
#include <Core/Core.h>
#include <Core/CraftObject.h>
#include <Math/Color.h>
#include <Math/Vector2.h>
#include <string>

namespace Craft
{
	//전방선언
	class Level;

	//가상 공간에 배치될 모든 액터의 기본 클래스, 확장 혀용
	class CRAFT_API Actor : public CraftObject
	{
		//매크로 지정 시 끝에 세미콜론 안붙임
		TYPE_DECLARATIONS(Actor, CraftObject)

	public:
		Actor(
			const std::string& image = "",
			const Vector2& position = Vector2::Zero,
			Color color = Color::White
		);

		virtual ~Actor();

		//게임 플레이 이벤트 함수
		virtual void BeginPlay(); //한번만 실행해야 하는 함수
		virtual void Tick(float deltaTime);
		virtual void Draw();

		//충돌 이벤트 함수
		virtual void OnCollision(const std::shared_ptr<Actor>& other);

		//액터 제거 함수
		void Destroy();

		//게임(엔진) 종료 함수
		void QuitGame();

		//Getter, Setter
		inline bool HasBeganPlay() const { //단순한 함수 inline처리
			return hasBeganPlay;
		}

		inline bool IsActive() const {
			return isActive && !hasExpired; //두 조건 모두 만족해야함
		}

		inline bool HasExpired() const{
			return hasExpired;
		}

		inline std::shared_ptr<Level> GetOwner() const {
			return owner.lock();
		}

		inline void SetOwner(std::weak_ptr<Level> newOwner) { owner = newOwner; }

		inline Vector2 GetPosition() const { return position; }

		void SetPosition(const Vector2& newPosition);

		Vector2 GetScreenSize() const;

		inline Vector2 GetPreviousPosition() const { return previousPosition; }

		//프레임 종료 후 이전 프레임 위치 저장 함수
		inline void SavePreviousState() { previousPosition = position; }

		//너비 반환 함수
		inline int GetWidth() const { return width; }

		inline void ChangeImage(const std::string& newImage) {
			//이미지 길이 설정
			width = static_cast<int>(newImage.length());

			//새로운 글자 값 설정
			image = newImage;
		}

		inline void ChangeIsActorStopedState() { isActorStoped = !isActorStoped; }

		inline void IsActorStoped() { if (isActorStoped) return; }
	
	protected:
		//BeginPlayer 이벤트 처리 여부 플래그
		bool hasBeganPlay = false;

		//액터의 활성화 여부
		bool isActive = true;

		//삭제 요청 여부 플래그 -> 삭제 요청 시 삭제 요청 객체를 모아서 특정 시점에 처리(garbage collector등)
		bool hasExpired = false;

		//게임 패배 혹은 승리 시 액터들 움직임을 정지시키기 위한 플래그 #새로 추가
		bool isActorStoped = false;

		//오너십 - 이 액터를 소유하는 레벨 객체, weak_ptr => 약한 참조 => 해당 위치가 유효한지 확인해야 함, 상호참조 방지를 위함(책임 분리)
		//Level은 이미 Actor객체들을 shared_ptr로 소유하고 있음 -> 여기서 Actor도 Level을 shared_ptr로 소유하는 경우
		//순환 참조 문제가 발생하여 Level혹은 Actor를 지워야 하는 경우 서로에 대한 참조가 남아있어 해제하지 못하고 메모리 누수 발생
		std::weak_ptr<Level> owner;

		//화면에 그릴 글자
		std::string image;

		//글자 색생
		Color color = Color::White;

		//글자 길이
		int width = 0;

		//렌더링 순서
		int sortingOrder = 0;

		//위치
		Vector2 position;

		//이전 프레임 위치
		Vector2 previousPosition;
	};


}
