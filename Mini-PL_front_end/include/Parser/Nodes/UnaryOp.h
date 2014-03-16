#pragma once

#ifndef UNARYOP_H
#define UNARYOP_H

#include "Parser/Nodes/Expression.h"
#include "Types/UnaryOperandType.h"

class UnaryOp : public Expression
{
public:
	UnaryOp(Expression* opnd) : expression(opnd), type(UnaryOperandType::no_operand)
	{ 
		valueType = opnd->valueType;
	};
	UnaryOp(UnaryOperandType t, Expression* opnd) : type(t), expression(opnd) { };
	virtual std::string evaluate(Evaluator& ev)
	{
		return ev.evaluate(*this);
	};
	virtual ValueType typeCheck(TypeChecker& tc) 
	{
		valueType = tc.typeCheck(*this);
		return valueType;
	};
	virtual ~UnaryOp() { delete expression; };
	Expression* expression;
	UnaryOperandType type;
};

#endif
