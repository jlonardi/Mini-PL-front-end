#pragma once

#ifndef STMNT_H
#define STMNT_H

#include "Parser/Nodes/Node.h"
#include <iostream>

class Statement : public Node
{
public:
	Statement() { };
	virtual void visit(Visitor& v) = 0;
	virtual ~Statement() { delete next; };
	Statement* next;
};

#endif
