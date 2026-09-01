#define NOMINMAX
#include "AStar.h"

AStar::AStar()
{}

AStar::~AStar()
{
	Clear();
}

std::vector<Vector2> AStar::FindPath(Vector2 startPos, Vector2 destinationPos)
{
	return std::vector<Vector2>();
}

void AStar::DisplayGridWithPath(std::vector<std::vector<int>>& grid, const std::vector<Vector2>& path)
{}

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
	return false;
}

Node* AStar::FindOpenNode(int x, int y) const
{
	return nullptr;
}

bool AStar::IsInClosedListI(int x, int y) const
{
	return false;
}

bool AStar::IsDestination(const Node* node) const
{
	return node != nullptr && targetNode != nullptr && node->position == targetNode->position;
}

void AStar::ClearVisualization(std::vector<std::vector<int>>& grid)
{}

void AStar::DisplayGrid(std::vector<std::vector<int>>&grid) const
{}
