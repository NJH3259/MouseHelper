#include "Level.h"

namespace Craft {
	Level::Level()
	{
		isLevelStoped = false;
	}

	Level::~Level()
	{}

	void Level::OnInitialized()
	{
		//초기화 되었다고 설정
		hasInitialized = true;
	}

	void Level::BeginPlay()
	{
		//액터 초기화 시에만 1회 호출되는 이벤트
		for(const std::shared_ptr<Actor> &actor : actorList){
			//이미 BeginPlay()가 호출된 경우 건너뛰기
			if (actor->HasBeganPlay()) {
				continue;
			}

			actor->BeginPlay();
		}
	}

	void Level::Tick(float deltaTime)
	{
		if (isLevelStoped)
		{
			return;
		}

		for (const std::shared_ptr<Actor>& actor : actorList) {
			
			if (!actor->IsActive()) {
				continue;
			}

			//Tick 이벤트 호출
			actor->Tick(deltaTime);
		}
	}

	void Level::Draw()
	{
		for (const std::shared_ptr<Actor>& actor : actorList) {

			if (!actor->IsActive()) {
				continue;
			}

			//Tick 이벤트 호출
			actor->Draw();
		}
	}

	void Level::ProcessAddAndDestroyActors()
	{
		//액터 제거 처리
		for (auto iterator = actorList.begin(); iterator != actorList.end();) {
			//제거 요청된 액터인지 확인
			auto actor = *iterator; //std::shared_ptr<Actor>타입
			if (actor->HasExpired()) {
				iterator = actorList.erase(iterator);
				continue;
			}

			//다음 순번을 처리하기 위해 이터레이터(반복자, 포인터) 증가 처리
			++iterator;
		}

		//액터 추가 처리
		//추가 요청 목록 없으면 종료
		if (addRequestedActorList.empty()) {
			return;
		}

		for (const auto& actor : addRequestedActorList) {
			actorList.emplace_back(actor);
		}

		//추가 처리된 목록 정리
		addRequestedActorList.clear();
	}

	void Level::SavePreviousActorStates()
	{
		//액터 순회하면서 이전 상태 저장
		for (const auto& actor : actorList) {
			if (!actor->IsActive()) {
				continue;
			}

			//상태 저장
			actor->SavePreviousState();
		}
	}

}