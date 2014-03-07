#pragma once

#ifndef NODE_H
#define NODE_H
#define null 0

#include <string>

enum class Type {undefined, identifier, integer, op_div, op_mult, op_plus, op_minus, op_less, colon, op_equal, op_and, 
				 op_not, string, op_assign, range, for_stmt, read, print, boolean, assert, single_expression,
				 statement, variable};

class Node
{
public:
	Type			type;
	std::string		value;
	Node*			left;
	Node*			right;

	Node();
	~Node();
};

#endif