#pragma once

#include <Level/Level.h>
#include <Input/Input.h>
#include <Util/Timer.h>
#include <Render/Renderer.h>

using namespace Craft;
class StageLevel : public Level
{
public:
	bool GetDebugMod() { return isDebugMod; }

	virtual void ResetLevel()
	{}

	std::vector<std::vector<int>> GetGridForPath() { return collisionGrid; }

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

		// 디버그 모드 시 A* 판정용 그리드 표시
		if (!isDebugMod)
		{
			Renderer::GetRenderer().SubmitTilemap(grid);
		}
		else
		{
			Renderer::GetRenderer().SubmitTilemap(collisionGrid, (0, 0), 4);
			Renderer::GetRenderer().Submit("On Debug Mod", (0, 0), Color::White, 9);
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

protected:
	Timer startTimer;

	float startDelay = 3.0f;

	bool isLevelStarted = false;

	bool isDebugMod = false;

	std::vector<std::vector<int>> grid;
	std::vector<std::vector<int>> collisionGrid;
};