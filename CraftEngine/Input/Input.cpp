#include "Input.h"
#include <cassert>
#include <Windows.h>

namespace Craft {
	//static 변수 별도 초기화
	Input* Input::instance = nullptr;
	
	Input::Input() {
		assert(!instance && "instance should be null");
		instance = this;
	}

	bool Input::GetKeyDown(int keyCode) const
	{
		return !keyStates[keyCode].wasKeyDown && keyStates[keyCode].isKeyDown;
	}

	bool Input::GetKeyUp(int keyCode) const
	{
		return keyStates[keyCode].wasKeyDown && !keyStates[keyCode].isKeyDown;
	}

	bool Input::GetKey(int keyCode) const
	{
		return keyStates[keyCode].isKeyDown;
	}

	Input& Input::Get()
	{
		//instance가 null이면 안됨
		assert(instance && "instance should not be null");
		return *instance;
	}

	void Input::ProcessInput()
	{
		//현재 프레임에 키 입력이 발생했는지 확인
		for (int ix = 0; ix < keyCount; ix++) {
			//키 눌림 여부 저장
			//0x8000은 최상위 비트
			//GetAsyncKeyState() => 입력이 없거나 오류 발생 시 0 반환
			//async -> 비동기처리: 게임은 대기 중 다른 동작을 처리해야 하기 때문에 비동기 많이 사용
			keyStates[ix].isKeyDown = ((GetAsyncKeyState(ix) & 0x8000) != 0);
		}
	}
	void Input::SavePreviousStates()
	{
		for (KeyState& state : keyStates) {
			//현재 프레임 입력 값을 이전 프레임 입력값으로 저장, 프레임 종료 직전 호출
			state.wasKeyDown = state.isKeyDown;
		}
	}
}