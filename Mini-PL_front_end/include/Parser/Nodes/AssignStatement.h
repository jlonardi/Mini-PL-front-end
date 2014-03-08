#pragma once

#ifndef ASSIGN_STATEMENT_H
#define ASSIGN_STATEMENT_H 

#include "Parser/Nodes/Statement.h"
#include <string>

class AssignStatement : public Statement
{
public:
	AssignStatement(std::string id, Expression* ex) : varID(id), expression(ex) { };
	virtual void visit(Visitor& v)
	{
		v.visit(*this);
	};
	virtual ~AssignStatement() { delete expression; };
	std::string varID;
	Expression* expression;
};

#endif