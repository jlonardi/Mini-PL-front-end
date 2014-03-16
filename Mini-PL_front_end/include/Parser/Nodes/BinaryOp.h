#pragma once

#ifndef BINARYOP_H
#define BINARYOP_H

#include "Parser/Nodes/Expression.h"
#include "Types/OperatorType.h"
#include <iostream>



class BinaryOp : public Expression
{
public:
	BinaryOp(OperatorType t, Expression* l, Expression* r) : type(t), lhs(l), rhs(r) 
	{
		
	};
	virtual std::string evaluate(Evaluator& ev)
	{
		return ev.evaluate(*this);
	};
	virtual ValueType typeCheck(TypeChecker& tc) 
	{
		valueType = tc.typeCheck(*this);
		return valueType;
	};
	virtual ~BinaryOp() { delete lhs; delete rhs; };
	OperatorType type;
	Expression* lhs;
	Expression* rhs;
};

#endif
