#pragma once

#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

class Variable : public Expression
{
public:
	std::string id;
	Variable(std::string val) : id(val) 
	{
		valueType = ValueType::undefined;
	}; 
	virtual std::string evaluate(Evaluator& ev)
	{
		return ev.evaluate(*this);
	};
	virtual ValueType typeCheck(TypeChecker& tc) 
	{
		valueType = tc.typeCheck(*this);
		return tc.typeCheck(*this);
	};
	virtual ~Variable() {};
private:
	Variable() {};
};

#endif