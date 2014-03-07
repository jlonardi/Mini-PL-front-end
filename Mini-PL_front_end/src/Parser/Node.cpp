#include "Parser/Node.h"

Node::Node() 
	: type(Type::undefined), left(null), right(null) {};

Node::~Node()
{
	delete left;
	delete right;
};