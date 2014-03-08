#pragma once

#ifndef NODE_H
#define NODE_H
#define null 0

#include "Interpreter/Visitor.h"

class Node
{
public:
	virtual ~Node() { };
	int lineNumber;
	int column;
};

#endif