#pragma once

#ifndef TYPECHECKER_H
#define TYPECHECKER_H

class IntegerConst;
class StringLiteral;
class Variable;
class BinaryOp;
class UnaryOp;
class BooleanConst;
enum class ValueType;

class TypeChecker
{
public:
	virtual ValueType typeCheck(const IntegerConst& node) = 0;
	virtual ValueType typeCheck(const StringLiteral& node) = 0;
	virtual ValueType typeCheck(const Variable& node) = 0;
	virtual ValueType typeCheck(BinaryOp& node) = 0;
	virtual ValueType typeCheck(UnaryOp& node) = 0;
	virtual ~TypeChecker() { };
};

#endif