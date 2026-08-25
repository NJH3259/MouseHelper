#include "Engine.h"
#include <Level/Level.h>
#include <Input/Input.h>
#include <Render/Renderer.h>
#include <Physics/CollisionSystem.h>

#include <iostream>
//#include <chrono> //시간 관련 헤더 파일, 유니티/언리얼에선 안씀
#include <Windows.h>
#include <cassert> // #include <assert.h>도 가능

namespace Craft
{
	//전역변수 초기화
	Engine* Engine::instance = nullptr; //static 변수 초기화

	Engine::Engine()
	{
		assert(!instance && "instance should be null");
		instance = this; //instance 초기화

		//엔진 설정 로드
		LoadEngineSetting();

		//입력 객체 생성
		input = std::make_unique<Input>(); //스마트 포인터 생성 함수

		//렌더러 객체 생성
		renderer = std::make_unique<Renderer>(Vector2(setting.width, setting.heigth));

		//콜리전 시스템 객체 생성
		collisionSystem = std::make_unique<CollisionSystem>();
	}
	Engine::~Engine()
	{
		instance = nullptr;
	}

	void Engine::Run()
	{
		//시간을 얻어오기 위한 고해상도 타이머 사용 -> 실 fps 측정
		// 밀리세컨드 - 1/1000초 -> 해상도 1000
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency); //해상도 구함 -> 해상도 천만
		
		LARGE_INTEGER counter;
		QueryPerformanceCounter(&counter);

		//프레임 계산을 위한 변수
		int64_t current = counter.QuadPart;
		int64_t previous = current;

		//고정 프레임으로 만들기 위한 값
		float oneFrameTime = 1.0f / setting.framerate;
	
		// 엔진 루프.
		while (true)
		{
			// 종료 조건 처리.
			if (isQuit)
			{
				break;
			}

			// 프레임 처리.

			// 입력 처리.
			ProcessInput();

			//프레임 시간 계산
			//1. 현재 시간 읽기
			QueryPerformanceCounter(&counter);
			current = counter.QuadPart;

			//2. (현재 시간 - 이전 시간)/시간 단위값(해상도) => 초단위로 변환
			//ex) 밀리세컨드(1/1000), 200밀리세컨드? -> 0.2초
			float deltaTime = static_cast<float>(current - previous) / static_cast<float>(frequency.QuadPart);

			//고정 프레임 처리
			//프레임 사이에 걸린 시간이 목표 시간보다 많이 지났으면 프레임 처리
			if (deltaTime >= oneFrameTime) { //매 프레임마다 화면에 그리고 이벤트 처리하도록

				//게임 플레이 로직
				// 게임 이벤트 함수 호출.
				OnInitialized();

				// 게임 이벤트의 초기화 함수(1번만 호출).
				BeginPlay();

				// 게임 업데이트.
				Tick(deltaTime);

				//충돌 처리
				ProcessCollision();

				// 화면 그리기.
				Draw();//여기까지 프레임 처리 완료

				//상태 저장 로직
				//레벨 전환 처리
				if (nextLevel) {
					//기존 레벨 정리
					if (mainLevel) {
						mainLevel.reset();
					}

					//추가 요청된 레벨을 메인 레벨로 설정
					mainLevel = nextLevel;

					//포인터 정리
					nextLevel.reset(); //포인터가 가리키는 곳을 nullptr로 리셋
				}

				// 추가/제거 요청된 액터 정리
				if (mainLevel) {
					mainLevel->ProcessAddAndDestroyActors();

					//액터의 이전 상태 저장: 충돌 판정을 위함
					mainLevel->SavePreviousActorStates();
				}

				// 입력 상태 저장.
				SavePreviousInputStates();

				//이전 시간에 현재 시간 저장
				previous = current;
			}

		}

		// 종료 처리 함수 호출.
		Shutdown();
	}

	void Engine::Quit()
	{
		// 엔진 종료 플래그 설정.
		isQuit = true;
	}

	//엔진 인스턴스 반환 함수
	Engine& Engine::Get()
	{

		//검증 assert => 반드시 통과해야하는 조건이 있을때 사용, 디버그에서만 동작
		assert(instance && "instance is null"); //&&뒤는 assert 메시지 출력

		return *instance;
	}

	void Engine::ProcessInput()
	{
		assert(input && "input should not be null");
		if (!input) {
			return;
		}
		input->ProcessInput();
	}

	void Engine::OnInitialized()
	{
		//레벨 초기화 처리
		//예외 처리
		if (!mainLevel || mainLevel->HasInitialized()) {
			return;
		}

		//초기화 이벤트 호출
		mainLevel->OnInitialized();
	}

	void Engine::BeginPlay()
	{
		if (!mainLevel) {
			return;
		}

		mainLevel->BeginPlay();
	}

	void Engine::Tick(float deltaTime)
	{
		//deltaTime 출력.
		//std::cout
		//	<< "Engine::Tick() - deltaTime: "
		//	<< deltaTime
		//	<< " | FPS: "
		//	<< (1.0f/deltaTime)
		//	<< "\n";

		if (!mainLevel) { //조건들은 분리해서 검사하는쪽이 depth도 낮고 가독성도 좋음
			return;
		}

		mainLevel->Tick(deltaTime);
	}

	void Engine::Draw()
	{
		if (!mainLevel) {
			return;
		}

		mainLevel->Draw();

		//렌더러에 Draw 이벤트 호출
		if (!renderer) {
			return;
		}
		renderer->Draw();
	}

	void Engine::ProcessCollision()
	{
		if (!mainLevel || !collisionSystem) {
			return;
		}

		//충돌 처리
		//의존성 주입(Dependency Injection) -> Solid pattern의 구현 방식 중 하나
		collisionSystem->ProcessCollision(mainLevel->actorList);
	}

	void Engine::SavePreviousInputStates()
	{
		assert(input && "input should not be null");
		if (!input) {
			return;
		}
		input->SavePreviousStates();
	}

	void Engine::Shutdown()
	{}

	void Engine::LoadEngineSetting()
	{
		//파일 열기
		FILE* file = nullptr;
		fopen_s(&file, "../Config/Setting.txt", "rt");

		//예외처리
		if (!file) {
			std::cout << "Fail to open engine setting file.\n";

			__debugbreak(); //디버그 모드에서 중단
			return;
		}

		//데이터 읽어오기
		const int bufferSize = 2048;
		char buffer[bufferSize] = {  };

		size_t readSize = fread(buffer, sizeof(char), bufferSize, file);

		//값 저장을 위해 서식 해석 (파싱 - Parsing)
		//문자열 자르기(split)
		char* context = nullptr;
		char* token = nullptr;

		//파일에서 읽은 전체 문자열을 개행 문자 기중으로 처음 자르기
		token = strtok_s(buffer, "\n", &context);

		//반복해서 자르기
		while (token) {
			// 공백 전까지 읽은 문자열을 저장할 변수
			char key[15] = {};

			//포맷을 지정한 문자열 읽기
			//공백 문자를 만나면 그 전까지 읽어서 저장
			sscanf_s(token, "%s", key, 15);

			//키 값을 비교해서 값 설정
			if (strcmp(key, "framerate") == 0) {
				sscanf_s(token, "framerate = %f", &setting.framerate);
			}
			else if (strcmp(key, "width") == 0) {
				sscanf_s(token, "width = %d", &setting.width);
			}
			else if (strcmp(key, "height") == 0) {
				sscanf_s(token, "height = %d", &setting.heigth);
			}

			//나머지 문자열 자르기(개행 문자 기준)
			token = strtok_s(nullptr, "\n", &context);
		}


		//파일 닫기
		fclose(file);
		file = nullptr;
	}
}