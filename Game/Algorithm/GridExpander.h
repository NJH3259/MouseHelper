#pragma once

#include <Math/Vector2.h>
#include "TileType.h"
#include <vector>

using namespace Craft;

class GridExpander
{
public:
	// 맵 그리드를 전달받고 액터의 크기를 고려해서 확장시킬 함수
	static std::vector<std::vector<int>> MakeAStarGrid(const std::vector<std::vector<int>>& grid, const Vector2 actorSize)
	{
		std::vector<std::vector<int>> newGrid = {};

		if (grid.empty())
		{
			return {};
		}

		newGrid = ExpandGrid(grid, actorSize);
		return newGrid;
	}

private:
	// 그리드에서 벽으로 표시된 부분(1) 위치를 기준으로 actorSize/2 만큼 두껍게 만듬
	static std::vector<std::vector<int>> ExpandGrid(const std::vector<std::vector<int>>& grid, const Vector2 actorSize)
	{
		std::vector<Vector2> offset = CalculateOffset(actorSize);

		// 반환할 newGrid생성
		std::vector<std::vector<int>> newGrid = {};
		newGrid.resize(grid.size());
		for (auto& row : newGrid)
		{
			row.resize(grid[0].size(), (int)TileType::Ground);
		}

		int gridYSize = grid.size();
		int gridXSize = grid[0].size() ;

		// 그리드 전체를 탐색하며 벽(1)을 탐색
		for (int y = 0; y < gridYSize; ++y)
		{
			for (int x = 0; x < gridXSize; ++x)
			{
				// 벽을 찾은 경우 벽 기준으로 offset만큼 확장시켜 새 그리드를 만듬
				if (grid[y][x] == (int)TileType::Wall)
				{
					// 벽 기준 offset만큼 확장, 범위 밖인 경우에는 확장하지 않음
					for (int iy = y - offset[1].x; iy <= y + offset[1].y; ++iy)
					{
						// y범위가 바깥으로 나간 경우
						if (iy < 0 || iy > gridYSize - 1)
						{
							continue;
						}

						// x범위가 바깥으로 나간 경우
						for (int ix = x - offset[0].x; ix <= x + offset[0].y; ++ix)
						{
							if (ix < 0 || ix > gridXSize - 1)
							{
								continue;
							}

							newGrid[iy][ix] = (int)TileType::Wall;
						}
					}
				}
			}
		}

		return newGrid;
	}

	static std::vector<Vector2> CalculateOffset(const Vector2 actorSize)
	{
		std::vector<Vector2> offset(2);

		// 액터의 width와 height가 홀수라 피봇 기준으로 나눠진 길이가 동일한 경우:
		if(actorSize.x % 2 == 1 && actorSize.y % 2 == 1)
		{
			int offsetX = actorSize.x / 2;
			int offsetY = actorSize.y / 2;
			
			offset[0] = Vector2(offsetX, offsetX);
			offset[1] = Vector2(offsetY, offsetY);

			return offset;
		}

		// 액터의 width 혹은 height가 짝수라 피봇 위치를 기준으로 나눠진 길이가 다른 경우
		else
		{
			int offsetX1 = actorSize.x / 2;
			int offsetY1 = actorSize.y / 2;

			int offsetX2 = 0;
			int offsetY2 = 0;

			// width만 짝수인 경우
			if (actorSize.x % 2 == 0 && actorSize.y % 2 == 1)
			{
				offsetX2 = offsetX1 - 1;
				offsetY2 = offsetY1;
			}

			// height만 짝수인 경우
			else if(actorSize.x % 2 == 1 && actorSize.y % 2 == 0)
			{
				offsetX2 = offsetX1;
				offsetY2 = offsetY1 - 1;
			}

			// width와 height 모두 짝수인 경우
			else
			{
				offsetX2 = offsetX1 - 1;
				offsetY2 = offsetY1 - 1;
			}

			offset[0] = Vector2(offsetX2, offsetX1);
			offset[1] = Vector2(offsetY2, offsetY1);

			return offset;
		}
	}
};

