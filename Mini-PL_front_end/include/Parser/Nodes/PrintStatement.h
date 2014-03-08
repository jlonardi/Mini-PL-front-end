#pragma once

#ifndef PRINT_STATEMENT_H
#define PRINT_STATEMENT_H

#include "Parser/Nodes/Statement.h"

class PrintStatement : public Statement
{
public:
	PrintStatement(Expression* ex) : expression(ex) { };
	virtual void visit(Visitor& v)
	{
		v.visit(*this);
	};
	virtual ~PrintStatement() { delete expression; };
	Expression* expression;
};

#endif