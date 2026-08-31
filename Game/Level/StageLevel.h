#pragma once

#include <Level/Level.h>
#include <Input/Input.h>
#include <Util/Timer.h>

using namespace Craft;
class StageLevel : public Level
{
protected:
	virtual void OnInitialized() override
	{
		Level::OnInitialized();

		startTimer.Reset();

		isLevelStoped = true;
		isLevelStarted = false;

		startTimer.SetTargetTime(startDelay);
	}

	virtual void Tick(float deltaTime) override
	{
		startTimer.Tick(deltaTime);

		WaitStartDelay();

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
	}

	void WaitStartDelay()
	{
		if (!isLevelStarted)
		{
			if (startTimer.IsTimeOut())
			{
				isLevelStoped = false;
				isLevelStarted = true;
			}
		}
	}

	virtual void ResetLevel()
	{}

private:
	Timer startTimer;

	float startDelay = 3.0f;

	bool isLevelStarted = false;
};