#pragma once

#include <vector>

class Node;

class AStar
{

private:
	std::vector<Node*> openList;
	std::vector<Node*> closedList;

	Node* targetNode;
	Node* startNode;
};

