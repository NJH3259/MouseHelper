#pragma once

#include <Math/Vector2.h>

class Node
{
public:
	Node(Craft::Vector2 position, Node* parentNode = nullptr)
		:position(position), gCost(0.0f), hCost(0.0f), fCost(0.0f), parentNode(parentNode)
	{
	}


	~Node()
	{
	}

private:
	Craft::Vector2 position;

	float gCost = 0.0f;
	float hCost = 0.0f;
	float fCost = 0.0f;

	Node* parentNode = nullptr;

};