#pragma once

#ifndef READ_STATEMENT_H
#define READ_STATEMENT_H

#include "Parser/Nodes/Statement.h"

class ReadStatement : public Statement
{
public:
	ReadStatement(std::string varName) : destionationVar(varName) { };
	virtual void visit(Visitor& v)
	{
		v.visit(*this);
	};
	std::string destionationVar;
};

#endif