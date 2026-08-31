#include "Game.h"
#include <Level/StageLevel.h>

#include <memory>

Game::Game()
{
	//레벨 생성 및 배열에 추가.
	//levelList.emplace_back(std::make_shared<Stage1>());

	// 시작 상태 설정.
	state = State::Title;

	// 게임 시작시 활성화할 레벨 설정.
	mainLevel = levelList[(int)state];
}

void Game::ToggleMenu(State gameState)
{
	int stateIndex = static_cast<int>(gameState);

	// 레벨 설정 및 상태 값 업데이트.
	nextLevel = levelList[stateIndex];
	state = static_cast<State>(gameState);
}

//외부에서 호출할 GameLevelReset함수
void Game::ResetGameLevel()
{
	ReInitilizeGameLevel();
}

//리셋 실행 로직은 Game에서 책임을 갖도록 private으로 감추기
void Game::ReInitilizeGameLevel()
{
	//std::shared_ptr<Stage1> testLevel = std::dynamic_pointer_cast<Stage1>(levelList[1]);
	//testLevel->ResetLevel();
	std::shared_ptr<StageLevel> currentLevel = std::dynamic_pointer_cast<StageLevel>(mainLevel);
	if (currentLevel)
	{
		currentLevel->ResetLevel();
	}
}
