#pragma once

#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "Parser/TypeChecker.h"
#include "Interpreter/Visitor.h"
#include "Types/OperatorType.h"
#include <vector>

struct Entry {
	std::string name;
	std::string value;
	ValueType type;
	ValueType declType;
};

class Statement;

class SemanticAnalyzer : public Visitor, public TypeChecker
{
public:
	SemanticAnalyzer() { };
	SemanticAnalyzer(Statement* node) : root(node), errors(false) { };
	virtual bool analyze();
	virtual ValueType typeCheck(UnaryOp& node);
protected:
	
	Statement* root;
	bool errors;
	std::vector<Entry> symbolTable;

	virtual void visit(VarDeclaration& node) ;
	virtual void visit(AssignStatement& node);
	virtual void visit(ForStatement& node);
	virtual void visit(ReadStatement& node);
	virtual void visit(PrintStatement& node);
	virtual void visit(AssertStatement& node) { };
	virtual ValueType typeCheck(const IntegerConst& node);
	virtual ValueType typeCheck(const StringLiteral& node);
	virtual ValueType typeCheck(const Variable& node);
	virtual ValueType typeCheck(BinaryOp& node);

	std::string typeToString(ValueType type);
	std::string operatorToString(OperatorType type);
};

#endif
