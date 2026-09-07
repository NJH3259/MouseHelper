#pragma once

#include <Level/Level.h>
#include <Input/Input.h>
#include <Util/Timer.h>
#include <Util/Util.h>
#include <Render/Renderer.h>
#include <GameManager/Game.h>

using namespace Craft;
class StageLevel : public Level
{
public:
	StageLevel()
	{
		stageClearText = Util::LoadImageFromFile("StageClear.txt", "../Assets/");
	}

	bool GetDebugMod() { return isDebugMod; }

	void ResetLevel()
	{
		hasInitialized = false;

		isStageCleared = false;

		actorList.clear();

		OnInitialized();
	}

	std::vector<std::vector<int>> GetGridForPath() { return collisionGrid; }

	void SetIsStageCleared(bool state) { isStageCleared = state; }

protected:
	virtual void OnInitialized() override
	{
		Level::OnInitialized();

		isDebugMod = false;

		startTimer.Reset();

		isLevelStoped = true;
		isLevelStarted = false;

		startTimer.SetTargetTime(startDelay);
	}

	virtual void Tick(float deltaTime) override
	{
		startTimer.Tick(deltaTime);

		WaitStartDelay(deltaTime);

		Level::Tick(deltaTime);

		// todo: 메뉴 화면 생성 및 메뉴로 이동
		if (Input::Get().GetKeyDown(VK_ESCAPE))
		{
			isLevelStoped = !isLevelStoped;
		}

		// 레벨 리셋 단축키
		if (Input::Get().GetKeyDown('R'))
		{
			ResetLevel();
			
		}

		// 디버그 모드 토글 단축키
		if (Input::Get().GetKeyDown('D'))
		{
			isDebugMod = !isDebugMod;
		}

		// 레벨 클리어 시 UI 출력
		//if (isStageCleared)
		{
			isLevelStoped = true;
			Renderer::GetRenderer().Submit(stageClearText, Vector2((grid[0].size() - stageClearText.length()/8) / 2 - 10, grid.size() / 2 - 3), Color::Yellow, 10);
		}

		// 디버그 모드 시 A* 판정용 그리드 표시
		if (!isDebugMod)
		{
			Renderer::GetRenderer().SubmitTilemap(grid);
		}
		else
		{
			Renderer::GetRenderer().SubmitTilemap(collisionGrid, (0, 0), 4);
			Renderer::GetRenderer().Submit("On Debug Mod", Vector2(0, 0), Color::White, 9);
		}
	}

	// 게임 레벨 시작 시 약간 딜레이를 주어 플레이어가 상황을 볼 수 있도록 하기 위한 함수
	void WaitStartDelay(float deltaTime)
	{
		if (!isLevelStarted)
		{
			if (startTimer.IsTimeOut())
			{
				isLevelStoped = false;
				isLevelStarted = true;
			}
			std::string leftTimeString = "Until Start: " + std::to_string(static_cast<int>(startDelay - startTimer.GetElapsedTime()) + 1);
			Renderer::GetRenderer().Submit(leftTimeString, Vector2(3, 4), Color::White, 5);
		}
	}

	void ChangeStage(State levelState)
	{
		Game& game = dynamic_cast<Game&>(Engine::Get());
		game.ResetGameLevel();
		game.ChangeLevel(levelState);
	}

protected:
	Timer startTimer;

	float startDelay = 3.0f;

	bool isLevelStarted = false;

	bool isDebugMod = false;

	bool isStageCleared = false;

	std::string stageClearText;

	std::vector<std::vector<int>> grid;
	std::vector<std::vector<int>> collisionGrid;
};