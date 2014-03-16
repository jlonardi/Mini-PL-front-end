#include "Interpreter/Interpreter.h"
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

void Interpreter::interprete()
{
	//std::cout << "Starting interpretation..." << std::endl << std::endl;
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
};

void Interpreter::visit(VarDeclaration& node)
{
	Entry entry;
	entry.name = node.id;
	entry.declType = node.declType;
	Expression* expr = node.expression;
	if(expr != null)
	{
		entry.value = node.expression->evaluate(*this);
	} else {
		entry.value = "";
		entry.type = ValueType::undefined;
	}
	symbolTable.push_back(entry);
};

void Interpreter::visit(AssignStatement& node)
{
	std::string varName = node.varID;
	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(varName))
		{
			symbolTable.at(i).value = node.expression->evaluate(*this);;
			symbolTable.at(i).type = node.expression->typeCheck(*this);
		}
	}
};

void Interpreter::visit(ForStatement& node)
{
	int from = std::stoi(node.from->evaluate(*this));
	int to = std::stoi(node.to->evaluate(*this));
	int x = 0;

	int tableIndex = 0;
	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(node.var))
		{
			if(symbolTable.at(i).type != ValueType::undefined)
			{
				x = std::stoi(symbolTable.at(i).value);
			}
			symbolTable.at(i).type = ValueType::number;
			symbolTable.at(i).value = intToString(x);
			tableIndex = i;
			break;
		}
	}

	while(x>=from && x<=to)
	{
		Statement* current = node.statements;
		while(current != null) 
		{
			current->visit(*this);
			current = current->next;
		}
		x++;
		symbolTable.at(tableIndex).value = intToString(x);
	};
};

void Interpreter::visit(ReadStatement& node)
{
	std::string varName = node.destionationVar;
	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(varName))
		{
			std::string value;
			std::cin >> value;
			std::cin.ignore();
			if(symbolTable.at(i).declType == ValueType::string_literal)
			{
				symbolTable.at(i).value = value;
				symbolTable.at(i).type = ValueType::string_literal;
			} else if(symbolTable.at(i).declType == ValueType::number)
			{ 
				try {
					symbolTable.at(i).value = intToString(std::stoi(value));
					symbolTable.at(i).type = ValueType::number;
				} catch(std::invalid_argument) {
					std::cout << "Invalid argument Exception! Could not set value \"" << value << "\" for variable " << varName <<"."<< std::endl;
					symbolTable.at(i).value = "";
					symbolTable.at(i).type = ValueType::undefined;
				}
				
			}
		}
	}
};

void Interpreter::visit(PrintStatement& node)
{
	std::cout << node.expression->evaluate(*this);
};

void Interpreter::visit(AssertStatement& node)
{
	std::string result = node.expression->evaluate(*this);
};

std::string Interpreter::evaluate(const IntegerConst& node)
{
	return node.value;
};

std::string Interpreter::evaluate(const StringLiteral& node)
{
	return node.value;
};
std::string Interpreter::evaluate(const Variable& node)
{
	std::string value;
	std::string varName =node.id;
	for(unsigned i=0; i<symbolTable.size(); i++)
	{
		if(!symbolTable.at(i).name.compare(varName))
		{
			if(symbolTable.at(i).type != ValueType::undefined)
			{
				return symbolTable.at(i).value;
			};
		}
	}
	return "Undefined";
};
std::string Interpreter::evaluate(BinaryOp& node)
{
	std::string lhsValue = node.lhs->evaluate(*this);
	std::string rhsValue = node.rhs->evaluate(*this);

	switch(node.typeCheck(*this))
	{
	case ValueType::number:
		{
			std::ostringstream val;
			switch(node.type)
			{
			case OperatorType::plus:
				val << std::stoi(lhsValue) + std::stoi(rhsValue);
				break;
			case OperatorType::minus:
				val << std::stoi(lhsValue) - std::stoi(rhsValue);
				break;
			case OperatorType::multiplication:
				val << std::stoi(lhsValue) * std::stoi(rhsValue);
				break;
			case OperatorType::division:
				val << std::stoi(lhsValue) / std::stoi(rhsValue);
				break;
			case OperatorType::less:
				val << std::boolalpha << (std::stoi(lhsValue) < std::stoi(rhsValue));
				break;
			case OperatorType::equal:
				val << std::boolalpha << (std::stoi(lhsValue) == std::stoi(rhsValue));
				break;
			case OperatorType::op_and:
				std::istringstream is(lhsValue + rhsValue);
				bool lhsBool, rhsBool;
				is >> std::boolalpha >> lhsBool;
				is >> std::boolalpha >> rhsBool;
				is.~is();
				val << std::boolalpha << (lhsBool && rhsBool);
				break;
			}

			return val.str();
		}
	case ValueType::string_literal:
		return lhsValue + rhsValue;
	default:
		return "Undefined";
	};
}

std::string Interpreter::evaluate(UnaryOp& node)
{
	return node.expression->evaluate(*this);
};

std::string Interpreter::intToString(int i)
{
	std::ostringstream oss;
	oss << i;
	return oss.str();
};