#pragma once

#include "Node.h"
#include <Math/Color.h>
#include <vector>
#include <algorithm>
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

	std::vector<Vector2> FindPath(const Vector2 startPos, const Vector2 destinationPos, std::vector<std::vector<int>>& grid);

	void DisplayPath(std::vector<std::vector<int>>& grid, const std::vector<Vector2>& path, Color color = Color::B_Red, const int iteratorIndex = 0);

private:
	void Clear();

	Node* CreateNode(const Vector2& position, Node* parent = nullptr);

	std::vector<Vector2> ConstructPath(Node* destination);

	float CalculateHeuristic(const Vector2 currentPos, const Vector2 destinationPos) const;

	bool IsValidGrid(const std::vector<std::vector<int>>& grid) const;

	bool IsInRange(int x, int y, std::vector<std::vector<int>>& grid);

	bool IsDiagonalBlocked(const Vector2& current, const Direction& direction, const std::vector<std::vector<int>>& grid) const;

	Node* FindOpenNode(int x, int y) const;

	bool IsInClosedList(int x, int y) const;

	bool IsDestination(const Node* node) const;

	void ClearVisualization(std::vector<std::vector<int>>& grid);
	
	// Heapify
	// 두 노드 중 어느 쪽이 힙에서 더 높은 우선순위를 갖는지 비교
	bool IsLower(const Node* a, const Node* b) const;

	// 새 이웃 노드를 OpenList 맨 끝에 추가 후 힙 정렬
	void HeapPush(Node* node);

	// fCost가 가장 작은 root노드를 꺼내고 마지막 원소를 루트로 옮긴 후 다시 힙 정렬, 다음 탐색 노드 꺼낼 때 호출
	Node* HeapPop();

	// 자식 노드가 부모 노드보다 우선순위 높은 동안 계속 교환
	void HeapSiftUp(int index);
	
	// 부모 노드가 자식 노드보다 우선순위 낮은 동안 계속 교환
	void HeapSiftDown(int index);

	// gCost 갱신 후 해당 노드만 sift up으로 재정렬
	void HeapDecreaseKey(Node* node);

private:
	std::vector<Node*> allocatedNodes;

	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	Node* targetNode = nullptr;
	Node* startNode = nullptr;
};

