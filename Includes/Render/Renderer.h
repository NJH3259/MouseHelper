#pragma once

#include <Core/Core.h>
#include <Math/Vector2.h>
#include <Math/Color.h>
#include <vector>
#include <string>
#include <memory>

namespace Craft
{	
	//전방 선언
	class ScreenBuffer;

	class CRAFT_API Renderer
	{
		//프레임(이미지) 데이터 구조체
		struct Frame {
			Frame(int bufferCount);
			~Frame();

			//프레임 초기화 함수
			void Clear(const Vector2& screenSize);

			//화면에 그릴 2차원 배열 문자값
			std::unique_ptr<CHAR_INFO[]> charInfoArray;

			//그리기 정렬 값 이차원 배열
			std::unique_ptr<int[]> sortingOrderArray;
		};

		struct RenderCommand {
			//화면에 그릴 문자값
			std::string img;

			//그릴 위치
			Vector2 position = Vector2::Zero;

			//색상
			Color color = Color::White;

			//그리기 정렬순서 -> 언리얼에서는 zorder
			//위치에 그림이 겹칠 경우 우선순위를 위함, 값이 크면 우선순위 높음
			int sortingOrder = -1;
		};

	public:
		Renderer(const Vector2& screenSize);
		~Renderer();

		//화면에 그릴 데이터 전달 함수
		void Submit(const std::string& image,
			const Vector2& position,
			const Color& color = Color::White,
			int sortingOrder = 0);

		//Draw이벤트 함수 - 엔진에서 호출
		void Draw();

		//Renderer 인스턴스 접근 함수
		static Renderer& GetRenderer();

	private:
		//그리기 작업 시작할 때 프레임(화면)을 지우는 함수
		void Clear();

		//전달 받은 렌더 명령을 활용해 화면을 그리는 함수
		//여기서 데이터를 만들어 내면 그걸 표시하는 함수로 전달
		void DrawRenderQueue();

		//그린 결과를 화면에 표시하는 함수
		void Present();

		//Getter
		const ScreenBuffer* const GetCurrentBuffer() const;

	private:
		//전역 접근 위한 변수
		static Renderer* instance;

		//이번 프레임에 그릴 렌더 명령을 모아둘 큐 배열
		std::vector<RenderCommand> renderQueue;

		//화면 크기
		Vector2 screenSize;

		//글자/그리기 순서 2차원 배열을 관리하느 프레임 객체
		std::unique_ptr<Frame> frame;

		//이중 버퍼링 구현을 위한 화면 버퍼 2개
		std::unique_ptr<ScreenBuffer> screenBufferArray[2];

		//버퍼 인덱스
		int currentBufferIndex = 0;
	};
}

