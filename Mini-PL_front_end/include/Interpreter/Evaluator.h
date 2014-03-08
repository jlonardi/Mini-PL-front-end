#pragma once

#ifndef EVALUATOR_H
#define EVALUATOR_H

#include <string>

class IntegerConst;
class StringLiteral;
class Variable;
class BinaryOp;
class UnaryOp;
class BooleanConst;

class Evaluator
{
public:
	virtual std::string evaluate(const IntegerConst& node) = 0;
	virtual std::string evaluate(const StringLiteral& node) = 0;
	virtual std::string evaluate(const Variable& node) = 0;
	virtual std::string evaluate(BinaryOp& node) = 0;
	virtual std::string evaluate(UnaryOp& node) = 0;
	virtual ~Evaluator() { };
};

#endif