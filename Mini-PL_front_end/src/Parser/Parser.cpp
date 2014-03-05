#include "Parser/Parser.h"
#include "Scanner/miniRules.h"
#include <iostream>

Parser::Parser(Scanner& scanner) 
	: m_scanner(scanner), m_status(Status::no_error), m_current_line(1), m_current_index(1) {};

bool Parser::parse()
{
	m_scanner.scan();
	
	prog();

	return true;
};

void Parser::prog()
{
	next();
	while(m_current_token.symbol != Symbol::end_of_text)
	{
		stmnts();
	};
}

void Parser::stmnts()
{
	stmnt();
	match(Symbol::semicolon, ";");
}

void Parser::stmnt()
{
	switch(m_current_token.symbol)
	{
	case Symbol::var:
		//std::cout << m_current_token.lexeme;
		next();
		match(Symbol::identifier, "<identifier>");
		match(Symbol::colon, "\":\"");
		type();
		if(m_current_token.symbol == Symbol::op_assign) {
			next();
			expr();
		};
		break;
	case Symbol::identifier:
		//std::cout << m_current_token.lexeme;
		next();
		match(Symbol::op_assign, ":=");
		expr();
		break;
	case Symbol::for_stmt:
		//std::cout << m_current_token.lexeme;
		next();
		match(Symbol::identifier, "<identifier>");
		match(Symbol::in, "in");
		expr();
		match(Symbol::range, "..");
		expr();
		match(Symbol::do_stmt, "do");
		while(m_current_token.symbol != Symbol::end)
		{
			stmnts();
		}
		next();
		match(Symbol::for_stmt, "for");
		break;
	case Symbol::read:
		//std::cout << m_current_token.lexeme;
		next();
		match(Symbol::identifier, "<identifier>");
		break;
	case Symbol::print:
		//std::cout << m_current_token.lexeme;
		next();
		expr();
		break;
	case Symbol::assert:
		//std::cout << m_current_token.lexeme;
		next();
		match(Symbol::lparen, "(");
		expr();
		match(Symbol::rparen, ")");
		break;
	default:
		std::cout << "Syntax error! Unexpected token at " << m_current_line << ":" << m_current_index << "." << std::endl;
		next();
	}
};

void Parser::expr()
{
	if(m_current_token.symbol == Symbol::op_not)
	{
		next();
	}

	opnd();

	if(is_operator()){
		op();
		opnd();
	}
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

void Parser::opnd()
{
	switch(m_current_token.symbol)
	{
	case Symbol::integer:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case Symbol::string:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case Symbol::identifier:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case Symbol::lparen:
		//std::cout << m_current_token.lexeme;
		next();
		expr();
		match(Symbol::rparen, ")");
		break;
	default:
		std::cout << "Unexpected operand at " << m_current_line << ":" << m_current_index << "." << std::endl;
		next();
	};
};

void Parser::op()
{
	switch(m_current_token.symbol)
	{
	case Symbol::op_plus :
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case Symbol::op_minus:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case Symbol::op_mult: 
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case  Symbol::op_div:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case Symbol::op_less:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case Symbol::op_equal:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case Symbol::op_and:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case Symbol::op_not:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	default:
		std::cout << "Unexpected operator at " << m_current_line << ":" << m_current_index << "." << std::endl;
	};
}

void Parser::type()
{
	switch(m_current_token.symbol)
	{
	case Symbol::integer:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case Symbol::string:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	case Symbol::boolean:
		//std::cout << m_current_token.lexeme;
		next();
		break;
	default:
		std::cout << "Unexpected type at " << m_current_line << ":" << m_current_index << "." << std::endl;
		m_status = Status::type_error;
	};
};

bool Parser::match(Symbol symbol, std::string tag)
{
	if(m_current_token.symbol == symbol)
	{
		//std::cout << m_current_token.lexeme;
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
			//std::cout << m_current_token.lexeme;
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
			//std::cout << m_current_token.lexeme;
			m_current_line++;
			m_current_index = 0;
		};
		m_current_token = m_scanner.nextToken();
	}

	m_current_index += m_current_token.lexeme.length();
};