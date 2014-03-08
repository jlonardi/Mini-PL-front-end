#pragma once

#ifndef OPERAND_H
#define OPERAND_H

#include "Parser/Nodes/Node.h"

class Operand : public Expression
{
public:
	Operand() { };
	virtual ~Operand() {};
	//OperandType type;
};

#endif