#include "Interpreter/Interpreter.h"
#include <iostream>

void Interpreter::interprete(Node* AST)
{
	if(AST == null) 
	{
		std::cout << "AST is null, something went really wrong!" << std::endl;
	} else {
		evaluateSubtree(AST);
	};
};

void Interpreter::evaluateSubtree(Node* node)
{
	if(node == null)
	{
		return;
	};
	Type type = node->type;
	switch(type)
	{
	case Type::statement: 
		{
			evaluateSubtree(node->left);
			evaluateSubtree(node->right);
			break;
		}
	case Type::variable:
		{
			std::string varName = node->left->value;
			Type type = node->right->type;
			bool inTable = false;
			for(std::vector<Entry>::iterator it = symbolTable.begin(); it != symbolTable.end(); ++it)
			{
				Entry e = *it;
				if(!e.name.compare(varName))
				{
					inTable = true;
					break;
				}
			};
			if(inTable)
			{
				std::cout << "Declaration of variable " << varName << " can be done only once!" << std::endl;
			} else {
				Entry e;
				e.name = varName;
				e.type = type;
				symbolTable.push_back(e);
			}
			break;
		}
	};
};