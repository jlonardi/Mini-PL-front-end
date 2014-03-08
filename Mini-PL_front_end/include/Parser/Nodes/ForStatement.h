#pragma once

#ifndef FOR_STMNT_H
#define FOR_STMNT_H

#include "Parser/Nodes/Statement.h"
#include <string>

class ForStatement : public Statement
{
public:
	ForStatement(std::string varName, Expression* f, Expression* t, Statement* stmnts) 
		: var(varName), from(f), to(t), statements(stmnts) { };
	virtual void visit(Visitor& v)
	{
		v.visit(*this);
	};
	std::string var;
	Expression* from;
	Expression* to;
	Statement* statements;
};

#endif