#define NOMINMAX
#include "AStar.h"
#include "TileType.h"
#include <Render/Renderer.h>

#include <vector>

/*	고려해야할 문제
	A* 알고리즘은 생각보다 무거움
	고양이 액터가 많아지면, 프로그램에 부하가 걸릴 가능성이 있음
	부하가 걸리면 전체 그리드를 정방형 타일로 나누고 A* 탐색 중
	해당 타일을 벗어나면 다시  AStar를 탐색해서 이동하도록 하여
	매 프레임마다 A*를 호출하는게 아니라 간격을 두어 부하를 줄일
	필요가 있을 수 있음 / 혹은 다른 방법 생각해보기
*/

AStar::AStar()
{}

AStar::~AStar()
{
	Clear();
}

std::vector<Vector2> AStar::FindPath(Vector2 startPos, Vector2 destinationPos, std::vector<std::vector<int>>& grid)
{
	// 초기화
	Clear();

	if (!IsValidGrid(grid))
	{
		return {};
	}

	if (!IsInRange(startPos.x, startPos.y, grid))
	{
		if (!IsInRange(destinationPos.x, destinationPos.y, grid))
		{
			return {};
		}

		return {};
	}

	if (grid[startPos.y][startPos.x] == (int)TileType::Wall || grid[destinationPos.y][destinationPos.x] == (int)TileType::Wall)
	{
		return {};
	}

	ClearVisualization(grid);

	startNode = CreateNode(startPos);
	targetNode = CreateNode(destinationPos);

	startNode->gCost = 0.0f;
	startNode->hCost = CalculateHeuristic(startPos, destinationPos);
	startNode->fCost = startNode->gCost + startNode->hCost;

	openList.emplace_back(startNode);

	// 편의를 위해 사전 비용 설정
	const float diagonalCost = 1.41421f;
	const std::vector<Direction> directions =
	{
		{Vector2(0, -1), 1.0f}, {Vector2(0, 1), 1.0f},  // 상하
		{Vector2(-1, 0), 1.0f}, {Vector2(1, 0), 1.0f},  // 좌우
		{Vector2(-1, -1), diagonalCost},         // 좌상단
		{Vector2(1, -1), diagonalCost},          // 우상단
		{Vector2(-1, 1), diagonalCost},          // 좌하단
		{Vector2(1, 1), diagonalCost}            // 우하단

	};

	while (!openList.empty())
	{
		Node* curNode = openList[0];
		for (Node* node : openList)
		{
			// 탐색할 노드 선택
			if (node->fCost < curNode->fCost || (node->fCost == curNode->fCost && node->hCost < curNode->hCost))
			{
				curNode = node;
			}
		}

		// 목표 노드인지 확인
		if (IsDestination(curNode))
		{
			return ConstructPath(curNode);
		}

		//현재 노드 처리
		auto iterator = std::find(openList.begin(), openList.end(), curNode);

		if (iterator != openList.end())
		{
			openList.erase(iterator);
		}

		closedList.emplace_back(curNode);

		// 인접한 노드 탐색
		for (const Direction& direction : directions)
		{
			Vector2 newPosition = curNode->position + direction.position;

			// 인접 노드가 이동할 수 있는지 확인
			if (!IsInRange(newPosition.x, newPosition.y, grid))
			{
				continue;
			}

			if (grid[newPosition.y][newPosition.x] == (int)TileType::Wall)
			{
				continue;
			}

			if (IsInClosedList(newPosition.x, newPosition.y))
			{
				continue;
			}

			// 새 이동 비용 계산
			float newGCost = curNode->gCost + direction.cost;
			
			// 탐색한 이웃 노드가 이미 OpenNode에 있는 경우 cost적으로 더 효율적인지 판단
			Node* openNode = FindOpenNode(newPosition.x, newPosition.y);
			if (openNode)
			{
				if (newGCost < openNode->gCost)
				{
					openNode->gCost = newGCost;
					openNode->fCost = openNode->gCost + openNode->hCost;
					openNode->parentNode = curNode;
				}

				continue;
			}

			// OpenNode목록에 없는 새로 탐색된 노드인 경우
			Node* neighborNode = CreateNode(newPosition, curNode);

			neighborNode->gCost = newGCost;
			neighborNode->hCost = CalculateHeuristic(neighborNode->position, targetNode->position);
			neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

			openList.emplace_back(neighborNode);
		}
	}

	//실패 시 빈 경로 반환
	return {};
}

void AStar::DisplayPath(std::vector<std::vector<int>>& grid, const std::vector<Vector2>& path, Color color)
{
	// 경로 표시를 위해 visited를 ground로 초기화
	ClearVisualization(grid);

	int iterator = 0;
	for (const Vector2 position : path)
	{
		if (iterator == 0 || iterator == path.size())
		{
			++iterator;
			continue;
		}

		// 시작점이나 도착점이 아닌 경우 Renderer에 path를 그리도록 전달
		Renderer::GetRenderer().Submit(" ", position, color, 5);

		++iterator;
	}
}

void AStar::Clear()
{
	for (Node* node : allocatedNodes)
	{
		delete node;
	}

	allocatedNodes.clear();
	openList.clear();
	closedList.clear();

	startNode = nullptr;
	targetNode = nullptr;
}

Node* AStar::CreateNode(const Vector2& position, Node* parent)
{
	Node* newNode = new Node(position, parent);
	allocatedNodes.emplace_back(newNode);

	return newNode;
}

std::vector<Vector2> AStar::ConstructPath(Node* destination)
{
	std::vector<Vector2> path;
	Node* current = destination;

	while (current)
	{
		path.emplace_back(current->position);
		current = current->parentNode;
	}

	std::reverse(path.begin(), path.end());

	return path;
}

float AStar::CalculateHeuristic(const Vector2 currentPos, const Vector2 destinationPos) const
{
	int diffX = std::abs(currentPos.x - destinationPos.x);
	int diffY = std::abs(currentPos.y - destinationPos.y);

	int diagonalDistance = std::min(diffX, diffY);
	int starightDistance = std::max(diffX, diffY);

	float diagonalCost = 1.41421;
	float starigthCost = 1.0f;

	return diagonalDistance * diagonalCost + starightDistance * starigthCost;
}

bool AStar::IsValidGrid(const std::vector<std::vector<int>>&grid) const
{
	if (grid.empty())
	{
		return false;
	}

	//가로줄의 길이
	size_t width = grid[0].size();
	for (const std::vector<int>& row : grid)
	{
		//앞에서 구한 행의 길이와 다르면 유효하지 않음
		if (row.size() != width)
		{
			return false;
		}
	}

	return true;
}

bool AStar::IsInRange(int x, int y, std::vector<std::vector<int>>& grid)
{
	// grid의 가로 크기는 같다고 가정.
	return x >= 0 && x < static_cast<int>(grid[0].size()) && y >= 0 && y < static_cast<int>(grid.size());
}

bool AStar::IsDiagonalBlocked(const Vector2& current, const Direction& direction, const std::vector<std::vector<int>>& grid) const
{
	if (direction.position.x == 0 || direction.position.y == 0)
	{
		return false;
	}

	int sideX = current.x + direction.position.x;
	int sideY = current.y + direction.position.y;

	//대각선: 좌우 + 상하 (ex. 왼쪽 위 대각선: 왼쪽 + 위쪽 => 왼쪽이나 위쪽 중 하나라도 장애물이 있으면 대각선으로 이동할 수 없음 (돌아가야함))
	return grid[current.y][sideX] == (int)TileType::Wall || grid[sideY][current.x] == (int)TileType::Wall;
}

Node* AStar::FindOpenNode(int x, int y) const
{
	for (Node* node : openList)
	{
		if (node->position == Vector2(x, y))
		{
			return node;
		}
	}
	return nullptr;
}

bool AStar::IsInClosedList(int x, int y) const
{
	for (Node* node : closedList)
	{
		if (node->position == Vector2(x, y))
		{
			return true;
		}
	}

	return false;
}

bool AStar::IsDestination(const Node* node) const
{
	return node != nullptr && targetNode != nullptr && node->position == targetNode->position;
}

void AStar::ClearVisualization(std::vector<std::vector<int>>& grid)
{
	for (std::vector<int>& row : grid)
	{
		for (int& value : row) {
			if (value == (int)TileType::Visited)
			{
				value = (int)TileType::Ground;
			}
		}
	}
}
