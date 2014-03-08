#include "Parser/SemanticAnalyzer.h"
#include "Parser/Nodes/Statement.h"
#include "Parser/Nodes/VarDeclaration.h"
#include "Parser/Nodes/PrintStatement.h"
#include "Parser/Nodes/ReadStatement.h"
#include "Parser/Nodes/AssignStatement.h"
#include "Parser/Nodes/ForStatement.h"
#include "Parser/Nodes/UnaryOp.h"
#include "Parser/Nodes/BinaryOp.h"
#include "Parser/Nodes/IntegerConst.h"
#include "Parser/Nodes/BooleanConst.h"
#include "Parser/Nodes/StringLiteral.h"
#include "Parser/Nodes/Variable.h"
#include "Parser/Nodes/AssertStatement.h"
#include <iostream>
#include <sstream>

bool SemanticAnalyzer::analyze()
{
	std::cout << std::endl << "Starting semantic analysis..."<< std::endl;
	if(root == null) 
	{
		std::cout << "AST is null, something went really wrong!" << std::endl;
	} else {
		Statement* current = root;
		while(current != null) 
		{
			current->visit(*this);
			current = current->next;
		}
	};
	return errors;
};

void SemanticAnalyzer::visit(VarDeclaration& node)
{
	Entry entry;
	entry.name = node.id;
	entry.declType = node.declType;
	Expression* expr = node.expression;
	if(expr != null)
	{
		entry.type = node.expression->typeCheck(*this);
		if(entry.type != entry.declType)
		{
			std::cout << "Error! Tried to assign expression type " << typeToString(entry.type) << " to variable type " 
				<< typeToString(entry.declType) << " at line " << node.lineNumber<< "." << std::endl;
			errors = true;
		};
	} else {
		entry.value = "";
		entry.type = ValueType::undefined;
	}
	symbolTable.push_back(entry);
};

void SemanticAnalyzer::visit(AssignStatement& node)
{
	std::string varName =node.varID;
	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(varName))
		{
			if(symbolTable.at(i).declType != node.expression->typeCheck(*this))
			{
				std::cout << "Trying to assign type " << typeToString(node.expression->typeCheck(*this)) 
					<< " to type " << typeToString(symbolTable.at(i).declType) << " at line " << node.lineNumber << "." << std::endl;
				errors = true;
			};
			symbolTable.at(i).type = node.expression->typeCheck(*this);
		}
	}
};

void SemanticAnalyzer::visit(ForStatement& node)
{
	int tableIndex = 0;
	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(node.var))
		{
			if(symbolTable.at(i).declType != ValueType::number)
			{
				std::cout << "Can't use " << typeToString(symbolTable.at(i).declType)
					<< " identifier as an iterator index at line " << node.lineNumber << "." << std::endl;
				errors = true;
			};
			symbolTable.at(i).type = ValueType::number;
		}
	}

	Statement* current = node.statements;
	while(current != null) 
	{
		current->visit(*this);
		current = current->next;
	}
};

void SemanticAnalyzer::visit(PrintStatement& node)
{
	if(node.expression->typeCheck(*this) == ValueType::undefined || node.expression->typeCheck(*this) == ValueType::undeclared)
	{
		std::cout << "Trying to print an " << typeToString(node.expression->typeCheck(*this)) << " identifier at line " << node.expression->lineNumber << std::endl;
		errors = true;
	}
};

ValueType SemanticAnalyzer::typeCheck(const IntegerConst& node)
{
	return ValueType::number;
};

ValueType SemanticAnalyzer::typeCheck(const StringLiteral& node)
{
	return ValueType::string;
};

ValueType SemanticAnalyzer::typeCheck(const Variable& node)
{
	std::string varName = node.id;
	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(varName))
		{
			ValueType type = symbolTable.at(i).type;
			//if(type == ValueType::undefined)
			//{
			//	std::cout << "Trying to use an undefiend variable " << node.id << " at line " << node.lineNumber << "." << std::endl;
			//	errors = true;
			//};
			return type;
		}
	}
	//std::cout << "Trying to use an undeclared variable " << node.id << " at line " << node.lineNumber << "." << std::endl;
	errors = true;
	return ValueType::undeclared;
};

ValueType SemanticAnalyzer::typeCheck(BinaryOp& node)
{
	if(node.lhs->typeCheck(*this) != node.rhs->typeCheck(*this))
	{
		std::cout << "Operator left hand side type does not match the right hand side at line " << node.lineNumber << "." << std::endl;
		errors = true;
		return ValueType::undefined;	// If types does not match the type is defined as undefined.
	}
	return node.lhs->typeCheck(*this);
};

ValueType SemanticAnalyzer::typeCheck(UnaryOp& node)
{
	return node.expression->typeCheck(*this);
};

std::string SemanticAnalyzer::typeToString(ValueType type)
{
	switch(type)
	{
	case ValueType::number:
		return "integer";
	case ValueType::string:
		return "string";
	case ValueType::undeclared:
		return "undeclared";
	default:
		return "undefined";
	};
};

void SemanticAnalyzer::printErorrMessage(std::string action, int line)
{

};