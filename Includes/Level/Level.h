#pragma once

#include <Actor/Actor.h> // Actor관리를 위함
#include <memory> // unique_ptr, shared_ptr 사용 => 스마트포인터: 사용자가 직접 해제하지 않아도 알아서 메모리 해제함
#include <vector> // Actor관리 및 동적 배열을 위함
#include <Core/Core.h>

namespace Craft
{
	//게임에 배치된 모든 액터를 관리하는 클래스
	// public std::enalble_shared_from_this<Level>
	// : shared_from_this() / weak_from_this()사용을 위해
	// : shared_from_this() -> this 포인터를 shared_ptr로 변환
	// : weak_from_this() -> this 포인터를 weak_ptr로 변환
	class CRAFT_API Level : public std::enable_shared_from_this<Level>, public CraftObject
	{
		//커스텀 타입 설정
		TYPE_DECLARATIONS(Level, CraftObject)

		//friend 선언 -> Level이 엔진 객체에 접근하도록 예외 처리
		friend class Engine;

	public:
		Level();
		virtual ~Level();

		//초기화 함수
		virtual void OnInitialized(); // -> 액터의 각 이벤트함수 호출

		//게임 플레이 이벤트 함수
		virtual void BeginPlay(); //한번만 실행해야 하는 함수
		virtual void Tick(float deltaTime);
		virtual void Draw();

		//Actor추가 함수 (탬플릿) -> 레벨에서 액터 관리하도록 업데이트 강의 돌려보기
		//Actor는 여러 타입으로 확장 가능하므로 typename T 사용
		// 가변인자: ...Args  ---> Args&& ...args로 처리 (함수 인자 전달을 유동적으로 함)
		//
		// is_base_of<Actor, T>::value -> T타입이 Actor의 하위 타입이라면 true를 반환하고 아니면 false
		// enable_if_t<> -> t의 조건이 <>를 true면 함수 호출 가능, false면 함수 호출 불가
		// enable_if_t<>의 <>가 is_base_of<Actor, T>::value이므로 최종적으로는 T가 Actor의 하위 구성(상속)이어야만 함수 호출 가능
		// **SFINAE기법 -> 해석 실패(Actor하위 클래스가 아니면)시 함수 없앰/치환 실패는 오류가 아니다**
		template<typename T, typename ...Args, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
		std::shared_ptr<T> SpawnActor(Args&& ...args) {
			std::shared_ptr<T> newActor = std::make_shared<T>(std::forward<Args>(args)...);
			//클래스 생성 시 클래스 파라미터가 가변적인데 제공하고 싶을 때 가변인자 Args 사용


			//추가 요청 목록에 포함
			addRequestedActorList.emplace_back(newActor);

			//오너십 설정: Level이 Actor를 소유하는 관계에 대한 설정
			newActor->SetOwner(weak_from_this());

			return newActor;
			

		}

		//Actor검색 함수 (탬플릿)
		template<typename T, typename = std::enable_if_t<std::is_base_of<Actor, T>::value>>
		std::shared_ptr<T> FindActor() {
			
			//검색 - 형변환
			for(const auto& actor : actorList)
			{
				std::shared_ptr<T> targetActor = std::dynamic_pointer_cast<T>(actor);
				
				if (targetActor) {
					return targetActor;
				}
			}

			//못찾은 경우 null반환
			return nullptr;
		}

		//Getter
		inline bool HasInitialized() const {
			return hasInitialized;
		}

	protected:
		//이전 프레임에 추가/제거 요청된 액터 처리 함수
		void ProcessAddAndDestroyActors();

		//액터의 이전 상태 처리 함수
		void SavePreviousActorStates();

	protected:
		//초기화 처리 여부 확인
		bool hasInitialized = false;

		//레벨에 배치된 모든 액터
		std::vector <std::shared_ptr<Actor>> actorList; //shared_ptr 소유권 이전이 가능한 unique_ptr

		//레벨에 추가 요청된 액터를 저장해두는 목록
		//현재 프레임을 처리하는 과정에서 액터 추가 요청 발생 시 해당 액터를 바로 추가하면 기존 액터 처리에 문제 발생 가능
		//=> 현재 프레임을 모두 처리한 후 요청된 액터를 actorList로 옮김
		std::vector <std::shared_ptr<Actor>> addRequestedActorList;

	};

}
