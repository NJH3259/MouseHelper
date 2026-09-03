#include "Renderer.h"
#include "ScreenBuffer.h"
#include <cassert>
#include <iostream>
#include <Windows.h>
#include <memory>

namespace Craft {

	//--------------------Frame-------------------//
	Renderer::Frame::Frame(int bufferCount) {
		//2차원 배열 생성
		charInfoArray = std::make_unique<CHAR_INFO[]>(bufferCount);
		sortingOrderArray = std::make_unique<int[]>(bufferCount);
	}

	Renderer::Frame::~Frame() {

	}

	//프레임 초기화 함수
	void Renderer::Frame::Clear(const Vector2& screenSize) {
		//이중 루프를 순회하며 값 초기화
		const int width = screenSize.x;
		const int height = screenSize.y;

		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				//1차원 배열을 2차원 배열로 사용할 때
				//필요한 인덱스 좌표 전환
				const int index = (y * width) + x;

				//글자 항목 초기화
				CHAR_INFO& info = charInfoArray[index];

				//빈문자 설정 - 기존 설정 값 지우기
				info.Char.AsciiChar = ' ';
				//색상 표기 안함
				info.Attributes = 0;

				//그리기 순서 배열 항목 초기화
				sortingOrderArray[index] = -1;
			}
		}
	}

	//--------------------Frame-------------------//

	//static 변수 초기화
	Renderer* Renderer::instance = nullptr;

	Renderer::Renderer(const Vector2& screenSize)
		: screenSize(screenSize)
	{
		assert(!instance && "instance should be null");
		instance = this;

		//프레임 객체 생성
		const int bufferCount = screenSize.x * screenSize.y;
		frame = std::make_unique<Frame>(bufferCount);

		//생성 후 프레임 지우기
		frame->Clear(screenSize);

		//이중 버퍼 구현을 위한 콘솔 버퍼 생성 및 초기화
		screenBufferArray[0] = std::make_unique<ScreenBuffer>(screenSize);
		screenBufferArray[0]->Clear();

		screenBufferArray[1] = std::make_unique<ScreenBuffer>(screenSize);
		screenBufferArray[1]->Clear();

		//화면에 0번 콘솔 버퍼 활성화
		SetConsoleActiveScreenBuffer(screenBufferArray[0]->GetBuffer());
	}

	Renderer::~Renderer()
	{
		instance = nullptr;

		//콘솔창 원래대로 복구
		SetConsoleActiveScreenBuffer(GetStdHandle(STD_OUTPUT_HANDLE));
	}

	void Renderer::Submit(const std::string & image, const Vector2 & position, const Color & color, int sortingOrder)
	{
		//렌더 명령 생성 및 값 설정
		//설계 관점에서 Actor냐 무엇이냐에 관계 없이 그리면 되기 때문에 책임을 끊어냄
		RenderCommand command;
		command.img = image;
		command.position = position;
		command.color = color;
		command.sortingOrder = sortingOrder;

		//Render Queue에 명령 추가
		renderQueue.emplace_back(command);
	}

	void Renderer::SubmitTilemap(const std::vector<std::vector<int>>& mapGrid, const Vector2& position, int sortingOrder)
	{
		RenderCommand command;
		command.isTilemap = true;
		command.grid = &mapGrid; // 맵 데이터의 주소만 전달 (복사 비용 0)
		command.sortingOrder = sortingOrder;

		renderQueue.emplace_back(command);
	}

	void Renderer::Draw()
	{
		//화면(이미지, 프레임) 지우기
		Clear();

		//프레임 그리기
		DrawRenderQueue();

		//화면 표시
		Present();
	}

	Renderer& Renderer::GetRenderer()
	{
		assert(instance && "instance should not be null");
		return *instance;
	}

	void Renderer::Clear()
	{
		//프레임 값 초기화
		frame->Clear(screenSize);

		//콘솔 버퍼 초기화
		GetCurrentBuffer()->Clear();
	}

	void Renderer::DrawRenderQueue()
	{
		for (const RenderCommand& command : renderQueue) {

			// 1. 타일맵 명령어인 경우
			if (command.isTilemap && command.grid != nullptr)
			{
				const auto& grid = *command.grid;
				int height = static_cast<int>(grid.size());
				if (height == 0) continue;

				int width = static_cast<int>(grid[0].size());

				int renderHeight = min(height, screenSize.y);
				int renderWidth = min(width, screenSize.x);

				for (int y = 0; y < renderHeight; ++y)
				{
					for (int x = 0; x < renderWidth; ++x)
					{
						int index = (y * screenSize.x) + x;

						// Sorting Order 비교
						if (frame->sortingOrderArray[index] > command.sortingOrder)
							continue;

						CHAR_INFO& info = frame->charInfoArray[index];
						int tileType = grid[y][x];

						if (tileType == 1) { // 벽
							info.Char.AsciiChar = ' ';
							info.Attributes = static_cast<WORD>(Color::B_White);
						}
						else if (tileType == 0) { // 바닥
							info.Char.AsciiChar = ' ';
							info.Attributes = 0;
						}
						else if (tileType == 2) { // A* 경로
							info.Char.AsciiChar = '.';
							info.Attributes = static_cast<WORD>(Color::Red);
						}

						frame->sortingOrderArray[index] = command.sortingOrder;
					}
				}
				continue; // 타일맵 처리 완료 후 다음 명령으로
			}

			//그릴 문자값이 없으면 건너뛰기
			if (command.img.empty()) {
				continue;
			}

			//이미지 출력할 때 위치 인덱스를 계산하기 위한 변수
			int xOffset = 0;
			int yOffset = 0;

			//문자열의 문자를 순회하면서 개행문자 탐색 및 인덱스 계산 알맞게 그림 그리기
			for (int sourceIndex = 0; sourceIndex < command.img.length(); ++sourceIndex) 
			{
				//이미지 중 문자가 개행 문자일시 yOffset + 1하고 그리지 않음
				if (command.img[sourceIndex] == '\n')
				{
					yOffset++;
					xOffset = 0;
					continue;
				}

				// 2. 현재 그리려는 문자의 화면 좌표 계산 (Offset 반영)
				int targetX = command.position.x + xOffset;
				int targetY = command.position.y + yOffset;

				if (0 > (command.position.y + yOffset) || (command.position.y + yOffset) > screenSize.y)
				{
					continue;
				}
				if (0 > (command.position.x + xOffset) || (command.position.x + xOffset) > screenSize.x)
				{
					continue;
				}

				//글자 2차원 배열의 인덱스
				//y*width + x
				const int index = ((command.position.y + yOffset) * screenSize.x) + command.position.x + xOffset;

				//개행 문자가 아닌경우 x축 방향으로 다음 칸으로 이동
				++xOffset;

				//정렬 순서를 비교해서 그릴지 말지 판정
				//이미 그려진 값이 우선순위가 높으면 건너뛰기
				if (frame->sortingOrderArray[index] > command.sortingOrder) {
					continue;
				}

				if (command.img[sourceIndex] == '@')
				{
					//2차원 배열에 글자, 속성 설정
					frame->charInfoArray[index].Char.AsciiChar = ' ';

					frame->charInfoArray[index].Attributes = static_cast<WORD>(Color::B_Blue);

					//그리기 우선순위 값 설정
					frame->sortingOrderArray[index] = command.sortingOrder;
				}
				
				else if (command.img[sourceIndex] == '.')
				{
					frame->charInfoArray[index].Char.AsciiChar = ' ';

					frame->charInfoArray[index].Attributes = static_cast<WORD>(Color::B_Green);

					//그리기 우선순위 값 설정
					frame->sortingOrderArray[index] = command.sortingOrder;
				}

				else if (command.img[sourceIndex] == '&')
				{
					frame->charInfoArray[index].Char.AsciiChar = ' ';

					frame->charInfoArray[index].Attributes = static_cast<WORD>(Color::B_Red);

					//그리기 우선순위 값 설정
					frame->sortingOrderArray[index] = command.sortingOrder;
				}

				else
				{
					//2차원 배열에 글자, 속성 설정
					frame->charInfoArray[index].Char.AsciiChar = command.img[sourceIndex];
				
					//글자 색상 설정
					frame->charInfoArray[index].Attributes = static_cast<DWORD>(command.color);
				
					//그리기 우선순위 값 설정
					frame->sortingOrderArray[index] = command.sortingOrder;
				}
			}

		}

		//앞에서 설정한 2차원 배열을 콘솔에 그리기
		GetCurrentBuffer()->Draw(frame->charInfoArray.get());

		//렌더큐 비우기
		renderQueue.clear();

		//콘솔 색상 초기화
		SetConsoleTextAttribute(GetCurrentBuffer()->GetBuffer(), static_cast<DWORD>(Color::White));
	}

	void Renderer::Present()
	{
		//현재 순번의 콘솔 버퍼를 활성화처리
		SetConsoleActiveScreenBuffer(GetCurrentBuffer()->GetBuffer());

		//인덱스 업데이트
		//One Minus 공식: 1 -> 0 -> 1 -> 0 -> ...
		currentBufferIndex = 1 - currentBufferIndex;
	}

	const ScreenBuffer* const Renderer::GetCurrentBuffer() const
	{
		return screenBufferArray[currentBufferIndex].get();
	}
}