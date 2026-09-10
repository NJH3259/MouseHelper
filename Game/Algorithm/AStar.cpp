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

	HeapPush(startNode);

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
		while (!openList.empty())
		{
			Node* curNode = HeapPop();

			if (IsDestination(curNode))
			{
				return ConstructPath(curNode);
			}

			closedList.emplace_back(curNode);

			for (const Direction& direction : directions)
			{
				Vector2 newPosition = curNode->position + direction.position;

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

				float newGCost = curNode->gCost + direction.cost;

				Node* openNode = FindOpenNode(newPosition.x, newPosition.y);
				if (openNode)
				{
					if (newGCost < openNode->gCost)
					{
						openNode->gCost = newGCost;
						openNode->fCost = openNode->gCost + openNode->hCost;
						openNode->parentNode = curNode;

						// 힙 전체 재정렬(make_heap) 대신, 이 노드만 O(log n)으로 재정렬
						HeapDecreaseKey(openNode);
					}

					continue;
				}

				Node* neighborNode = CreateNode(newPosition, curNode);

				neighborNode->gCost = newGCost;
				neighborNode->hCost = CalculateHeuristic(neighborNode->position, targetNode->position);
				neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

				HeapPush(neighborNode);
			}
		}


		//실패 시 빈 경로 반환
		return {};
	}
}

void AStar::DisplayPath(std::vector<std::vector<int>>& grid, const std::vector<Vector2>& path, Color color, const int iteratorIndex)
{
	// 경로 표시를 위해 visited를 ground로 초기화
	ClearVisualization(grid);

	int iterator = 0;
	for (const Vector2 position : path)
	{
		if (iterator == 0 || iterator < iteratorIndex || iterator == path.size())
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
	openList.clear();   // 힙이든 아니든 vector::clear()로 충분
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

bool AStar::IsLower(const Node* a, const Node* b) const
{
	// fCost가 작은 쪽이 우선이고
	if (a->fCost != b->fCost)
	{
		return a->fCost < b->fCost;
	}

	// fCost가 같으면 hCost가 작은 쪽을 우선으로 함
	return a->hCost < b->hCost;
}

void AStar::HeapPush(Node* node)
{
	// 오픈 리스트에 추가
	openList.emplace_back(node);

	// 위치 힙 인덱스에 기록
	node->heapIndex = static_cast<int>(openList.size()) - 1;

	// Heapify 진행
	HeapSiftUp(node->heapIndex);
}

Node* AStar::HeapPop()
{
	// 오픈 리스트의 루트를 꺼내고 맨 끝 노드를 루트로 이동
	Node* root = openList[0];
	Node* last = openList.back();

	openList[0] = last;
	last->heapIndex = 0;
	openList.pop_back();

	// 꺼낸 루트는 Open상태가 아니므로 힙 인덱스 초기화
	root->heapIndex = -1;

	if (!openList.empty())
	{
		HeapSiftDown(0);
	}

	return root;
}

void AStar::HeapSiftUp(int index)
{
	while (index > 0)
	{
		int parentIndex = (index - 1) / 2;

		// 부모가 더(또는 동등하게) 우선순위 높으면 더 올라갈 필요 없음 → 종료
		if (!IsLower(openList[index], openList[parentIndex]))
		{
			break;
		}

		std::swap(openList[index], openList[parentIndex]);
		openList[index]->heapIndex = index;
		openList[parentIndex]->heapIndex = parentIndex;

		index = parentIndex;
	}
}

void AStar::HeapSiftDown(int index)
{
	int size = static_cast<int>(openList.size());

	while (true)
	{
		int left = index * 2 + 1;
		int right = index * 2 + 2;
		int smallest = index;

		// 왼쪽 자식이 존재하고 현재보다 우선순위 높으면 후보로 선택
		if (left < size && IsLower(openList[left], openList[smallest]))
		{
			smallest = left;
		}
		// 오른쪽 자식이 존재하고 지금까지의 후보보다 우선순위 높으면 갱신
		if (right < size && IsLower(openList[right], openList[smallest]))
		{
			smallest = right;
		}

		// 자식들보다 현재 노드가 이미 우선순위 높으면 힙 조건 만족 → 종료
		if (smallest == index)
		{
			break;
		}

		std::swap(openList[index], openList[smallest]);
		openList[index]->heapIndex = index;
		openList[smallest]->heapIndex = smallest;

		index = smallest;
	}
}

// 오픈 리스트의 노드의 gCost/fCost가 더 작게 갱신됬을 때 호출
// 비용 감소 -> 우선순위 증가 -> 트리 구조 상 위쪽으로 이동
void AStar::HeapDecreaseKey(Node * node)
{
	HeapSiftUp(node->heapIndex);
}
