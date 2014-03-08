#pragma once

#ifndef VAR_DECLARATION_H
#define VAR_DECLARATION_H

#include "Parser/Nodes/Statement.h"
#include "Parser/Nodes/Expression.h"
#include <string>
#include <iostream>

//enum class DeclarationType {integer, string, boolean, undefined};

class VarDeclaration : public Statement
{
public:
	VarDeclaration(std::string id, Expression* expr, ValueType t) : id(id), expression(expr), declType(t) { };
	virtual void process() 
	{
		std::cout << "I am a Variable Declaration." << std::endl;
	};
	virtual void visit(Visitor& v)
	{
		v.visit(*this);
	};
	virtual ~VarDeclaration() { delete expression; };
	std::string id;
	Expression* expression;
	ValueType declType;
};

#endif