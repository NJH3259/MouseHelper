#include "Cat.h"
#include <Level/StageLevel.h>
#include <Actor/Mouse.h>
#include <Input/Input.h>
#include <Util/Util.h>
#include <Render/Renderer.h>

#include<cassert>
#include <memory>

using namespace Craft;

Cat::Cat(const Vector2 position, Color color)
	:Actor(" ", position, color)
{
	ChangeImage(Util::LoadImageFromFile("Cat.txt", "../Assets/"));

	moveTimer.SetTargetTime(0.16f);

	sortingOrder = 1;

	isActorStoped = false;

	pivot = Vector2(position.x + (int)(GetWidth() / 2), position.y + (int)(GetHeight() / 2));
}

void Cat::BeginPlay()
{
	if (!mouse) {
		mouse = FindMouseInLevel();
	}

	if (gridForPath.empty())
	{
		gridForPath = std::dynamic_pointer_cast<StageLevel>(GetOwner())->GetGridForPath();
	}
}

void Cat::Tick(float deltaTime)
{
	Actor::Tick(deltaTime);

	if (isActorStoped)
	{
		return;
	}

	moveTimer.Tick(deltaTime);

	pivot = Vector2(position.x + (int)(GetWidth() / 2), position.y + (int)(GetHeight() / 2));


	if(!isHolded)
	{
		if(prevHolded)
		{
			// 마우스를 따라가기 위한 오프셋 초기화
			isSetOffset = false;
			curOffset = Vector2(0, 0);

			// 마우스 클릭을 때서 내려놓은 곳에 벽이 있는 경우 (그리드와 피봇이 겹치는 경우)

			// 레벨에 position을 받아 해당 위치에 벽이 있는지 반환하는 함수 구현하기

			// 반환값을 보고 현재 위치에 벽이 있다면

			//방법 1. 가장 가까운, 벽과 겹치지 않는 위치로 이동시킨다.(채택)
			//방법 2. 위치를 초기화 시킨다. -> 방법 2는 게임 난이도 조절에 악영향을 미칠 수 있음

			//이 연산을 Tick에서 매번 수행하면 혹시나 벽과 겹치는 상황을 없앨 수 있지만, 매 Tick마다 연산을 진행하여 성능이 떨어질 수 있음 -> 고민해볼 문제

			if (IsPlacedOnWall(pivot, gridForPath))
			{
				MoveToClosestGround(pivot, gridForPath);
			}

			// 이전 잡힘 상태 갱신
			prevHolded = false;
		}

		// A*로 탐색 및 이동
		MoveToMouse();
	}
	else
	{
		// 이전 잡힘 상태 갱신
		if (!prevHolded)
		{
			prevHolded = true;
		}

		//플레이어에게 잡힌 상태면 마우스 위치 따라가기
		if (!isSetOffset)
		{
			curOffset = CalculateOffset(GetMousePosition());
			isSetOffset = true;
		}

		position = GetMousePosition() - curOffset;
	}

	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
	if (ISDEBUGMOD)
	{
		// 피봇 위치 화면에 표시
		Renderer::GetRenderer().Submit(" ", pivot, Color::B_Purple, 7);

		if (isHolded)
		{
			std::string currentPos = "잡힌 고양이의 위치: (" + std::to_string(position.x) + ", " + std::to_string(position.y) + ")";
			Renderer::GetRenderer().Submit(currentPos, Vector2(2, 4), Color::White, 5);
			std::string pivotPos = "액터의 피봇 위치: (" + std::to_string(pivot.x) + ", " + std::to_string(pivot.y) + ")";
			Renderer::GetRenderer().Submit(pivotPos, Vector2(2, 5), Color::White, 5);
		}
	}
	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
}

void Cat::OnCollision(const std::shared_ptr<Actor>&other)
{
	
	if(!isHolded)
	{
		// 쥐와 충돌한 경우
		if (Cast<Mouse>(other))
		{
			// 레벨 실패 처리
			std::dynamic_pointer_cast<StageLevel>(GetOwner())->SetIsLevelFailed();
		}
	}
}

void Cat::MoveToMouse()
{
	assert(mouse && "mouse should not be null");

	//플레이어에게 붙잡힌 상태가 아니라면
	if (!isHolded)
	{
		// 매 프레임마다 이동은 지나치게 빠르므로 이동은 제한 시간을 두고 이동한다.
		if(moveTimer.IsTimeOut())
		{
			//탐색한 mouse를 향해 A*알고리즘으로 경로 탐색
			path.clear();
			path = catPathFinder.FindPath(pivot, mouse->GetPivot(), gridForPath);

			// 피봇이 mouse의 피봇과 완전히 겹치는 경우 path의 size는 1이다(자기 자신의 위치만 들어있음)
			if (path.size() > 1)
			{
				// mouse를 향해 최적 경로로 한칸 이동
				position = path[1] - Vector2((int)(GetWidth() / 2), (int)(GetHeight() / 2));
			}
			moveTimer.Reset();
		}
	}

	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
	if (ISDEBUGMOD)
	{
		//디버그 모드인 경우 경로 그리기
		catPathFinder.DisplayPath(gridForPath, path);
	}
	//-----------------------------------------------------------Debug Mod--------------------------------------------------------//
}

Craft::Vector2 Cat::GetMousePosition()
{
	return Input::Get().GetMousePosition();
}

std::shared_ptr<Mouse> Cat::FindMouseInLevel()
{
	std::shared_ptr<Mouse> mouse = nullptr;
	
	//Level의 ActorList에서 Mouse 탐색
	mouse = GetOwner()->FindActor<Mouse>();

	assert(mouse && "mouse should not be null");

	return mouse;
}

Vector2 Cat::CalculateOffset(const Vector2 mousePosition)
{

	int offsetX = mousePosition.x - position.x;
	int offsetY = mousePosition.y - position.y;

	Vector2 offset = Vector2(offsetX, offsetY);

	return offset;
}

bool Cat::IsPlacedOnWall(const Vector2 pivot, const std::vector<std::vector<int>>& grid)
{
	// 그리드 범위를 벗어난 경우 벽으로 취급 (MoveToClosestGround가 그리드 안으로 끌어옴)
	if (pivot.y < 0 || pivot.y >= grid.size() || pivot.x < 0 || pivot.x >= grid[0].size())
	{
		return true;
	}

	// 피봇 위치를 기준으로 판정용 그리드 위에 있는지 확인, 1 = TileType::Wall
	if (grid[pivot.y][pivot.x] == 1)
	{
		return true;
	}

	return false;
}

void Cat::MoveToClosestGround(Craft::Vector2& pivot, const std::vector<std::vector<int>>& grid)
{
	Vector2 minPosition = Vector2(grid[0].size(), grid.size());
	int bigger = minPosition.x > minPosition.y ? minPosition.x : minPosition.y;

	// index 크기의 범위 내에 땅이 있는지 확인
	for (int index = 1; index < bigger; ++index)
	{
		// minPosition 탐색을 위한 플래그
		bool found = false;
		float minDist = std::sqrt(2* (bigger * bigger));

		// index * index크기의 사각형을 피봇 위치에 만들어서 사각형 범위 탐색
		for (int iy = pivot.y - index; iy <= pivot.y + index; ++iy)
		{
			// 인덱스가 그리드를 넘어간 경우 다시 탐색
			if (iy < 0 || iy >= grid.size())
			{
				continue;
			}

			for (int ix = pivot.x - index; ix <= pivot.x + index; ++ix)
			{
				// 인덱스가 그리드를 넘어간 경우 다시 탐색
				if (ix < 0 || ix >= grid[0].size())
				{
					continue;
				}

				// 해당 위치가 벽이면 다시 반복문 탐색
				if (grid[iy][ix] == 1)
				{
					continue;
				}

				// 피봇부터 (ix, iy)까지 거리가 피봇부터 minPosition까지의 거리가 짧다면 minPosition을 (ix, iy)로 대체
				Vector2 delta = pivot - Vector2(ix, iy);
				float deltaDist = std::sqrt((delta.x * delta.x) + (delta.y * delta.y));

				// 최적 위치를 못찾았거나 거리가 더 짧은 최적 위치를 새로 찾은 경우
				if(!found || deltaDist < minDist)
				{
					found = true;
					minDist = deltaDist;
					minPosition = Vector2(ix, iy);
				}
			}
		}

		// 찾은 경우 최적 위치로 pivot을 옮겨서 위치 수정
		if (found)
		{
			pivot = minPosition;
			return;
		}
	}
}
