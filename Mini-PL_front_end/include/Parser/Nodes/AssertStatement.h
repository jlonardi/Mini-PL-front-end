#pragma once

#ifndef ASSERT_STATEMENT_H
#define ASSERT_STATEMENT_H

#include "Parser/Nodes/Statement.h"

class AssertStatement : public Statement
{
public:
	AssertStatement(Expression* ex) : expression(ex) { };
	virtual void visit(Visitor& v)
	{
		v.visit(*this);
	};
	Expression* expression;
};

#endif