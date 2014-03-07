#pragma once

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Parser/Node.h"
#include <vector>

struct Entry {
	std::string name;
	Type type;
};

class Interpreter
{
public:
	Interpreter() {};
	void interprete(Node* AST);
private:
	void evaluateSubtree(Node* node);
	std::vector<Entry> symbolTable;
};

#endif