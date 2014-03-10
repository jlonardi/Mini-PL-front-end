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
	std::cout << "Starting semantic analysis..." << std::endl;
	if(root == null) 
	{
		return true; // There is an error
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

	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(node.id))
		{
			std::cout << "Redefinition of identifier " << node.id << " at line " << node.lineNumber << "." << std::endl;
			errors = true;
		}
	}

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
	std::string varName = node.varID;
	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(varName))
		{
			ValueType type = node.expression->typeCheck(*this);
			if(symbolTable.at(i).declType != type)
			{
				std::cout << "Trying to assign type " << typeToString(type) 
					<< " to type " << typeToString(symbolTable.at(i).declType) << " at line " << node.lineNumber << "." << std::endl;
				errors = true;
			};
			symbolTable.at(i).type = type;
		}
	}
};

void SemanticAnalyzer::visit(ForStatement& node)
{
	int tableIndex = 0;
	bool found = false;
	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(node.var))
		{
			if(symbolTable.at(i).declType != ValueType::number)
			{
				std::cout << "Can't use " << typeToString(symbolTable.at(i).declType)
					<< " identifier as an index at line " << node.lineNumber << "." << std::endl;
				errors = true;
			};
			symbolTable.at(i).type = ValueType::number;
			found = true;
		}
	}

	if(!found)
	{
		std::cout << "Can't use undeclared identifier \"" << node.var << "\" as an index at line " << node.lineNumber << "." << std::endl;
		errors = true;
	};

	ValueType type = node.from->typeCheck(*this);

	if(type == ValueType::undefined || type == ValueType::undeclared)
	{
		std::cout << "Can't use " << typeToString(type)
			<< " as the range value \"from\" at line " << node.from->lineNumber << "." << std::endl;
		errors = true;
	};

	type = node.to->typeCheck(*this);

	if(type == ValueType::undefined || type == ValueType::undeclared)
	{
		std::cout << "Can't use " << typeToString(type)
			<< " as the range value \"to\" at line " << node.to->lineNumber << "." << std::endl;
		errors = true;
	};

	Statement* current = node.statements;
	while(current != null) 
	{
		current->visit(*this);
		current = current->next;
	}
};

void SemanticAnalyzer::visit(ReadStatement& node)
{
	bool found = false;
	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(node.destionationVar))
		{
			found = true;
			symbolTable.at(i).type = symbolTable.at(i).declType;
			break;
		}
	}

	if(!found)
	{
		std::cout << "Trying to read into an undeclared variable." << std::endl;
	};
}

void SemanticAnalyzer::visit(PrintStatement& node)
{
	ValueType type = node.expression->typeCheck(*this);
	if(type == ValueType::undefined || type == ValueType::undeclared)
	{
		std::cout << "Trying to print an " << typeToString(type) << " identifier at line " << node.lineNumber << std::endl;
		errors = true;
	}
};

ValueType SemanticAnalyzer::typeCheck(const IntegerConst& node)
{
	return ValueType::number;
};

ValueType SemanticAnalyzer::typeCheck(const StringLiteral& node)
{
	return ValueType::string_literal;
};

ValueType SemanticAnalyzer::typeCheck(const Variable& node)
{
	std::string varName = node.id;
	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(varName))
		{
			ValueType type = symbolTable.at(i).type;
			return type;
		}
	}
	return ValueType::undeclared;
};

ValueType SemanticAnalyzer::typeCheck(BinaryOp& node)
{
	ValueType lhsType = node.lhs->typeCheck(*this);
	ValueType rhsType = node.rhs->typeCheck(*this);
	if(lhsType != rhsType)
	{
		//std::cout << "Operator left hand side type does not match the right hand side at line " << node.lineNumber << "." << std::endl;
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
	case ValueType::string_literal:
		return "string";
	case ValueType::undeclared:
		return "undeclared";
	default:
		return "undefined";
	};
};