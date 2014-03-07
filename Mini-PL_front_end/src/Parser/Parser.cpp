#include "Parser/Parser.h"
#include "Scanner/miniRules.h"
#include <iostream>

Parser::Parser(Scanner& scanner) 
	: m_scanner(scanner), m_status(Status::no_error), m_current_line(1), m_current_index(1) {};

Node* Parser::parse()
{
	m_scanner.scan();

	return prog();
};

Node* Parser::prog()
{
	next();
	return stmnts();
}

Node* Parser::stmnts()
{
	Node* stmntNode = stmnt();
	match(Symbol::semicolon, ";");
	Node* tail = stmnts_tail();

	Node* stmntsNode = createParentNode(Type::statement,stmntNode,tail);
	return stmntsNode;
}

Node* Parser::stmnts_tail()
{	if(m_current_token.symbol == Symbol::end || m_current_token.symbol == Symbol::end_of_text)
{
	return null;	// return epsilon
};
Node* node = stmnts();
return node;
}

Node* Parser::stmnt()
{
	Node* node = null;
	Node* valueNode;
	Node* ex;
	Node* range;
	Node* from;
	Node* to;
	Node* statements;
	Node* variableNode;
	Node* varTypeNode ;
	Type t;

	switch(m_current_token.symbol)
	{
	case Symbol::var:
		std::cout << m_current_token.lexeme;
		next();
		valueNode = createNode(Type::identifier, m_current_token.lexeme);
		match(Symbol::identifier, "<identifier>");
		match(Symbol::colon, "\":\"");
		
		t = type();
		varTypeNode = createNode(t, "");
		variableNode = createParentNode(Type::variable, valueNode, varTypeNode);
		if(m_current_token.symbol == Symbol::op_assign) {
			next();
			ex = expr();
			node = createParentNode(Type::op_assign, variableNode, ex);
		} else {
			node = variableNode;	
		};
		break;
	case Symbol::identifier:
		std::cout << m_current_token.lexeme;
		valueNode = createNode(Type::identifier, m_current_token.lexeme);
		next();
		match(Symbol::op_assign, ":=");
		ex = expr();
		node = createParentNode(Type::op_assign, valueNode, ex);
		break;
	case Symbol::for_stmt:
		std::cout << m_current_token.lexeme;
		next();
		match(Symbol::identifier, "<identifier>");
		match(Symbol::in, "in");
		from = expr();
		match(Symbol::range, "..");
		to = expr();
		range = createParentNode(Type::range, from, to);
		match(Symbol::do_stmt, "do");
		statements = stmnts();
		node = createParentNode(Type::for_stmt, range, statements);
		next();
		match(Symbol::for_stmt, "end");
		match(Symbol::for_stmt, "for");
		break;
	case Symbol::read:
		std::cout << m_current_token.lexeme;
		valueNode = createNode(Type::identifier, m_current_token.lexeme);
		next();
		node = createParentNode(Type::read, valueNode, null);
		match(Symbol::identifier, "<identifier>");
		break;
	case Symbol::print:
		std::cout << m_current_token.lexeme;
		next();
		ex = expr();
		node = createParentNode(Type::print, ex, null);
		break;
	case Symbol::assert:
		std::cout << m_current_token.lexeme;
		next();
		match(Symbol::lparen, "(");
		ex = expr();
		node = createParentNode(Type::assert, ex, null);
		match(Symbol::rparen, ")");
		break;
	default:
		std::cout << "Syntax error! Unexpected token at " << m_current_line << ":" << m_current_index << "." << std::endl;
		next();
	}
	return node;
};

Node* Parser::expr()
{
	Type t;
	Node* node = null;
	if(m_current_token.symbol == Symbol::op_not)
	{
		t = Type::op_not;
		node = createParentNode(Type::op_not, null, null);
		next();
	}

	Node* expr;

	Node* opnd1 = opnd();

	if(is_operator()){
		t = op();
		Node* opnd2 = opnd();
		expr = createParentNode(t, opnd1, opnd2);
	} else {
		expr = createParentNode(Type::single_expression, opnd1, null);
	}

	if(node != null)
	{
		node->left = expr;	// If there was the "not" operator make the expression as the op node left child
	} else {
		node = expr;		// Otherwise the node is the expression itself
	}

	return node;
};

bool Parser::is_operator()
{
	switch(m_current_token.symbol)
	{
	case Symbol::op_plus :
		return true;
		break;
	case Symbol::op_minus:
		return true;
		break;
	case Symbol::op_mult: 
		return true;
		break;
	case  Symbol::op_div:
		return true;
		break;
	case Symbol::op_less:
		return true;
		break;
	case Symbol::op_equal:
		return true;
		break;
	case Symbol::op_and:
		return true;
		break;
	case Symbol::op_not:
		return true;
		break;
	default:
		return false;
	};
};

Node* Parser::opnd()
{
	Node* node = null;
	switch(m_current_token.symbol)
	{
	case Symbol::integer:
		std::cout << m_current_token.lexeme;
		node = createNode(Type::integer, m_current_token.lexeme);
		next();
		break;
	case Symbol::string:
		std::cout << m_current_token.lexeme;
		node = createNode(Type::string, m_current_token.lexeme);
		next();
		break;
	case Symbol::identifier:
		std::cout << m_current_token.lexeme;
		node = createNode(Type::identifier, m_current_token.lexeme);
		next();
		break;
	case Symbol::lparen:
		std::cout << m_current_token.lexeme;
		next();
		node = expr();
		match(Symbol::rparen, ")");
		break;
	default:
		std::cout << "Unexpected operand at " << m_current_line << ":" << m_current_index << "." << std::endl;
		next();
	};

	return node;
};

Type Parser::op()
{
	Type t;
	switch(m_current_token.symbol)
	{
	case Symbol::op_plus :
		std::cout << m_current_token.lexeme;
		t = Type::op_plus;
		next();
		break;
	case Symbol::op_minus:
		std::cout << m_current_token.lexeme;
		t = Type::op_minus;
		next();
		break;
	case Symbol::op_mult: 
		std::cout << m_current_token.lexeme;
		t = Type::op_mult;
		next();
		break;
	case  Symbol::op_div:
		std::cout << m_current_token.lexeme;
		t = Type::op_div;
		next();
		break;
	case Symbol::op_less:
		std::cout << m_current_token.lexeme;
		t = Type::op_less;
		next();
		break;
	case Symbol::op_equal:
		std::cout << m_current_token.lexeme;
		t = Type::op_equal;
		next();
		break;
	case Symbol::op_and:
		std::cout << m_current_token.lexeme;
		t = Type::op_and;
		next();
		break;
	case Symbol::op_not:
		std::cout << m_current_token.lexeme;
		t = Type::op_not;
		next();
		break;
	default:
		std::cout << "Unexpected operator at " << m_current_line << ":" << m_current_index << "." << std::endl;
		t = Type::undefined;
	};
	return t;
}

Type Parser::type()
{
	Type t;
	switch(m_current_token.symbol)
	{
	case Symbol::integer:
		std::cout << m_current_token.lexeme;
		t = Type::integer;
		next();
		break;
	case Symbol::string:
		std::cout << m_current_token.lexeme;
		t = Type::string;
		next();
		break;
	case Symbol::boolean:
		std::cout << m_current_token.lexeme;
		t = Type::boolean;
		next();
		break;
	default:
		std::cout << "Unexpected type at " << m_current_line << ":" << m_current_index << "." << std::endl;
		m_status = Status::type_error;
		t = Type::undefined;
	};

	return t;
};

bool Parser::match(Symbol symbol, std::string tag)
{
	if(m_current_token.symbol == symbol)
	{
		std::cout << m_current_token.lexeme;
		next();
		return true;
	} 
	std::cout << "Unexpected token at "  << m_current_line << ":" << m_current_index <<  ", expected token " << tag << "." << std::endl;
	m_status = Status::syntax_error;
	return false;
};

void Parser::handleError()
{

};

void Parser::next()
{
	m_current_token = m_scanner.nextToken();

	while(m_current_token.symbol == Symbol::whitespace || m_current_token.symbol == Symbol::newline ||  m_current_token.symbol == Symbol::comment)
	{
		if(m_current_token.symbol == Symbol::whitespace)
		{
			std::cout << m_current_token.lexeme;
			m_current_index++;
		}
		if(m_current_token.symbol == Symbol::comment)
		{
			m_current_index += m_current_token.lexeme.length();
			//check how many newlines there is in the comment
			for(unsigned i=0; i<m_current_token.lexeme.length(); i++)
			{
				if(m_current_token.lexeme.at(i) == '\n')
				{
					m_current_line++;
					m_current_index = 0;
				};
			}
		};
		if(m_current_token.symbol == Symbol::newline)
		{
			std::cout << m_current_token.lexeme;
			m_current_line++;
			m_current_index = 0;
		};
		m_current_token = m_scanner.nextToken();
	}

	m_current_index += m_current_token.lexeme.length();
};

Node* Parser::createParentNode(Type type, Node* left, Node* right)
{
	Node* n = new Node;
	n->type = type;
	n->left = left;
	n->right = right;
	return n;
};

Node* Parser::createNode(Type type, std::string value)
{
	Node* n = new Node;
	n->type = type;
	n->value = value;

	return n;
};