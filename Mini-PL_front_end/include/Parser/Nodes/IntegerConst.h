#pragma once

#ifndef INTEGERCONST_H
#define INTEGERCONST_H

#include "Parser/Nodes/Expression.h"

class IntegerConst : public Expression
{
public:
	std::string value;
	IntegerConst(std::string val) : value(val) 
	{
		valueType = ValueType::number;
	};
	virtual std::string evaluate(Evaluator& ev)
	{
		return ev.evaluate(*this);
	};
	virtual ValueType typeCheck(TypeChecker& tc) 
	{
		return tc.typeCheck(*this);
	};
	virtual ~IntegerConst() {};
private:
	IntegerConst() {};
};

#endif