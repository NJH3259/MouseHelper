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

		//일시 정지 단축키 todo: 메뉴 화면 생성 및 메뉴로 이동
		if (Input::Get().GetKeyDown(VK_ESCAPE))
		{
			isLevelStoped = !isLevelStoped;
		}

		//레벨 리셋 단축키
		if (Input::Get().GetKeyDown('R'))
		{
			ResetLevel();
			
		}

		if (Input::Get().GetKeyDown('D'))
		{
			//todo: 디버그 모드 구현
			isDebugMod = !isDebugMod;
		}
	}

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

	std::vector<std::vector<int>> MakeAStarGrid()
	{

	}

protected:
	Timer startTimer;

	float startDelay = 3.0f;

	bool isLevelStarted = false;

	bool isDebugMod = false;

	std::vector<std::vector<int>> grid;
	std::vector<std::vector<int>> newGrid;
};