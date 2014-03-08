#pragma once

#ifndef STRINGCONST_H
#define STRINGCONST_H

#include <string>

class StringLiteral : public Expression
{
public:
	std::string value;
	StringLiteral(std::string val) : value(val) 
	{
		valueType = ValueType::string;
	};
	virtual std::string evaluate(Evaluator& ev)
	{
		return ev.evaluate(*this);
	};
	virtual ValueType typeCheck(TypeChecker& tc) 
	{
		return tc.typeCheck(*this);
	};
	virtual ~StringLiteral() {};
private:
	StringLiteral() {};
};

#endif