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
	bool GetIsStageCleared() { return isStageCleared; }

	void SetIsLevelFailed() { isLevelFailed = true; }

	int GetManipulationCount() { return mouseManipulationCount; }
	void UseManipulation() { mouseManipulationCount -= 1; }

protected:
	virtual void OnInitialized() override
	{
		Level::OnInitialized();

		isDebugMod = false;

		startTimer.Reset();

		isLevelPaused = false;

		isLevelFailed = false;

		isLevelStoped = true;
		isLevelStarted = false;

		startTimer.SetTargetTime(startDelay);
	}

	virtual void Tick(float deltaTime) override
	{
		startTimer.Tick(deltaTime);

		WaitStartDelay(deltaTime);

		Level::Tick(deltaTime);

		if (mouseManipulationCount < 0)
		{
			assert(mouseManipulationCount >= 0);
		}

		Renderer::GetRenderer().Submit("Remaining Move Count: " + std::to_string(mouseManipulationCount), Vector2((grid[0].size() - stageClearText.length() / 14) / 2 + 1, 2), Color::BrightWhite, 10);

		// 일시정지는 레벨 시작 후 가능
		if(isLevelStarted)
		{
			// 일시정지 토글
			if (Input::Get().GetKeyDown(VK_ESCAPE))
			{
				if (isLevelPaused)
				{
					isLevelStoped = false;
				}
				else
				{
					isLevelStoped = true;
				}

				isLevelPaused = !isLevelPaused;
			}
		}

		if (isLevelFailed)
		{
			isLevelStoped = true;
			isLevelPaused = true;
		}

		if(Input::Get)

		// 디버그 모드 토글 단축키
		if (Input::Get().GetKeyDown('D'))
		{
			isDebugMod = !isDebugMod;
		}

		if (Input::Get().GetKeyDown('N'))
		{
			isStageCleared = true;
		}

		if (isLevelPaused)
		{
			if (isLevelFailed)
			{
				Renderer::GetRenderer().Submit("You Lose", Vector2((grid[0].size() - stageClearText.length() / 14) / 2 + 8, grid.size() / 2 - 3), Color::BrightWhite, 10);
			}
			else
			{
				Renderer::GetRenderer().Submit("Pause", Vector2((grid[0].size() - stageClearText.length() / 14) / 2 + 10, grid.size() / 2 - 3), Color::BrightWhite, 10);
			}
			Renderer::GetRenderer().Submit("Press ESC to Resume Game", Vector2((grid[0].size() - stageClearText.length() / 14) / 2 + 1, grid.size() / 2), Color::BrightWhite, 10);
			Renderer::GetRenderer().Submit("Press R to Restart Stage", Vector2((grid[0].size() - stageClearText.length() / 14) / 2 + 1, grid.size() / 2 + 1), Color::BrightWhite, 10);
			Renderer::GetRenderer().Submit("Press T to Return to Title", Vector2((grid[0].size() - stageClearText.length() / 14) / 2, grid.size() / 2 + 2), Color::BrightWhite, 10);

			// 레벨 리셋 단축키
			if (Input::Get().GetKeyDown('R'))
			{
				ResetLevel();
			}

			// 타이틀로 복귀
			if (Input::Get().GetKeyDown('T'))
			{
				ResetLevel();
				dynamic_cast<Game&>(Engine::Get()).ChangeLevel(State::Title);
			}
		}

		// 레벨 클리어 시 UI 출력
		if (isStageCleared)
		{
			for (auto actor : actorList)
			{
				actor->ChangeIsActorStopedState(true);
			}

			Renderer::GetRenderer().Submit(stageClearText, Vector2((grid[0].size() - stageClearText.length()/8) / 2 - 10, grid.size() / 2 - 3), Color::Yellow, 10);
			Renderer::GetRenderer().Submit("Press Mouse L Button To Play Next Stage", Vector2((grid[0].size() - stageClearText.length() / 8) / 2, grid.size() / 2 + 3), Color::BrightWhite, 10);
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
			Renderer::GetRenderer().Submit(leftTimeString, Vector2(grid[0].size()/ 2 - 4, grid.size() / 2), Color::White, 5);
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

	// 레벨 일시 정지를 위한 플래그, 레벨 멈춤 상태와는 별도로 일시 정지 처리만을 위함
	bool isLevelPaused = false;

	// 레벨 실패 처리를 위한 플래그
	bool isLevelFailed = false;

	bool isDebugMod = false;

	bool isStageCleared = false;

	int mouseManipulationCount = 0;

	std::string stageClearText;

	std::vector<std::vector<int>> grid;
	std::vector<std::vector<int>> collisionGrid;
};