#pragma once

#include <Math/Vector2.h>

class Node
{
public:
	Node(Craft::Vector2 position, Node* parentNode = nullptr)
		:position(position), parentNode(parentNode)
	{
	}

public:
	Craft::Vector2 position;

	float gCost = 0.0f;
	float hCost = 0.0f;
	float fCost = 0.0f;

	Node* parentNode = nullptr;

};