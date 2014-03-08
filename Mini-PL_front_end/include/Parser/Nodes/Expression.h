#pragma once

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Parser/Nodes/Node.h"
#include "Parser/TypeChecker.h"
#include "Interpreter/Evaluator.h"

enum class ExpressionType { unary, operation };
enum class ValueType {number, string, boolean, undefined, undeclared};

class Expression : public Node
{
public:
	ExpressionType exprType;
	ValueType valueType;
	virtual std::string evaluate(Evaluator& ev) = 0;
	virtual ValueType typeCheck(TypeChecker& tc) = 0;
};

#endif