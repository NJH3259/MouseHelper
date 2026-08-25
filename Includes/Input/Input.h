#pragma once

#include <Core/Core.h>

namespace Craft
{
	class CRAFT_API Input
	{
		friend class Engine;

		//키 입력 데이터 저장용 구조체
		struct KeyState
		{
			//현재 프레임에 키가 눌렸는지 여부
			bool isKeyDown = false;

			//이전 프레임에 키가 눌렸는지 여부
			bool wasKeyDown = false;
		};

	public:
		Input();
		~Input() = default;

		//키 눌림/해제 여부 확인 함수
		//이전 프레임에 안눌렸다가 이번 프레임에 눌리면 true를 한번만 반환
		bool GetKeyDown(int keyCode) const;

		//이전 프레임에 눌렸다가 이번 프레임에 안눌리면 true반환
		bool GetKeyUp(int KeyCode) const;

		//현재 프레임에 입력이 눌리면 반복해서 true반환
		bool GetKey(int keyCode) const;

		//외부에서 접근이 가능하도록 하는 함수(싱글톤)
		static Input& Get();

	private:
		//현재 프레임에 특정 키 입력이 발생했는지 처리하는 함수
		void ProcessInput();

		//이전 프레임의 키 눌림 상태를 저장하는 함수
		void SavePreviousStates();

	private:
		//가상 키의 수 (처리할 키의 수)
		const int keyCount = 256;

		//각 키 상태를 관리할 배열
		KeyState keyStates[256] = {  };

		//전역 접근이 가능한 변수
		static Input* instance;
	};
}