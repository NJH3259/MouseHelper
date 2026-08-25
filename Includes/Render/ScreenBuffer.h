#pragma once

#include <Math/Vector2.h>
#include <Windows.h>

//이중 버퍼링 구현을 위한 화면 버퍼 클래스

namespace Craft
{
	class ScreenBuffer
	{
	public:
		ScreenBuffer(const Vector2& screenSize);
		~ScreenBuffer();

		//화면 지우기
		void Clear() const;

		//전달된 글자 값 그리는 함수
		void Draw(const CHAR_INFO* const charInfo) const;

		//Getter
		inline HANDLE GetBuffer() const { return buffer; }



	private:
		//화면 버퍼 핸들
		HANDLE buffer = nullptr;

		//화면 크기
		Vector2 size;
	};
}

