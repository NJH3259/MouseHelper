#pragma once

#include "Node.h"
#include <vector>
#include <memory>

using namespace Craft;

class AStar
{
private:
	struct Direction
	{
		Vector2 position = Vector2(0, 0);
		float cost = 0.0f;
	};

public:
	AStar();
	~AStar();

	std::vector<Vector2> FindPath(Vector2 startPos, Vector2 destinationPos);

	void DisplayGridWithPath(std::vector<std::vector<int>>& grid, const std::vector<Vector2>& path);

private:
	void Clear();

	Node* CreateNode(const Vector2& position, Node* parent = nullptr);

	std::vector<Vector2> ConstructPath(Node* destination);

	float CalculateHeuristic(const Vector2 currentPos, const Vector2 destinationPos) const;

	bool IsValidGrid(const std::vector<std::vector<int>>& grid) const;

	bool IsInRange(int x, int y, std::vector<std::vector<int>>& grid);

	bool IsDiagonalBlocked(const Vector2& current, const Direction& direction, const std::vector<std::vector<int>>& grid) const;

	Node* FindOpenNode(int x, int y) const;

	bool IsInClosedListI(int x, int y) const;

	bool IsDestination(const Node* node) const;

	void ClearVisualization(std::vector<std::vector<int>>& grid);

	void DisplayGrid(std::vector<std::vector<int>>& grid) const;

private:
	std::vector<Node*> allocatedNodes;

	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	Node* targetNode = nullptr;
	Node* startNode = nullptr;
};

