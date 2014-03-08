#pragma once

#ifndef BOOLEANCONST_H
#define BOOLEANCONST_H

#include "Parser/Nodes/Expression.h"

class BooleanConst : public Expression
{
public:
	std::string value;
	BooleanConst(std::string val) : value(val) 
	{
		valueType = ValueType::number;
	};
	virtual std::string evaluate(Evaluator& ev)
	{
//		valueType = ev.typeCheck(*this);
//		return ev.evaluate(*this);
	};
	virtual ~BooleanConst() {};
private:
	BooleanConst() {};
};

#endif