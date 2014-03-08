#pragma once

#ifndef STMNT_H
#define STMNT_H

#include "Parser/Nodes/Node.h"
#include <iostream>

enum class StatementType {assert, assign, for_stmnt, print, read, declaration, undefined};

class Statement : public Node
{
public:
	Statement() : type(StatementType::undefined) { };
	virtual void visit(Visitor& v) = 0;
	virtual ~Statement() { delete next; };
	Statement* next;
	StatementType type;
};

#endif