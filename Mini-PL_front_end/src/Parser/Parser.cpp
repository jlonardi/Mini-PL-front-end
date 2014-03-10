#include "Parser/Parser.h"
#include "Parser/Nodes/IntegerConst.h"
#include "Parser/Nodes/StringLiteral.h"
#include "Parser/Nodes/Variable.h"
#include "Parser/Nodes/BinaryOp.h"
#include "Parser/Nodes/UnaryOp.h"
#include "Parser/Nodes/ReadStatement.h"
#include "Parser/Nodes/PrintStatement.h"
#include "Parser/Nodes/AssertStatement.h"
#include "Parser/Nodes/ForStatement.h"
#include "Scanner/miniRules.h"
#include <iostream>
#include <sstream>

Parser::Parser(Scanner& scanner) 
	: m_scanner(scanner), m_status(Status::no_error), m_current_line(1), m_current_index(1), error(false) {};

Statement* Parser::parse()
{
	m_scanner.scan();
	//std::cout << "Starting to parse the source..." << std::endl;

	Statement* AST = prog();
	if(error)
	{
		return null;
	} else {
		return AST;
	}
};

Statement* Parser::prog()
{
	next();
	return stmnts();
}

Statement* Parser::stmnts()
{
	Statement* statement = null;
	try {
		statement = stmnt();
		match(Symbol::semicolon, ";");
		statement->next = stmnts_tail();
	} catch( std::exception& e ) {
		std::cerr << e.what();
		handleError();

		if(statement == null)
		{
			statement = stmnts_tail();
		} else {
			statement->next = stmnts_tail();
		}
	};


	return statement;
}

Statement* Parser::stmnts_tail()
{	
	if(m_current_token.symbol == Symbol::end || m_current_token.symbol == Symbol::end_of_text)
	{
		return null;	// return epsilon
	};
	Statement* node = stmnts();
	return node;
}

Statement* Parser::stmnt()
{
	switch(m_current_token.symbol)
	{
	case Symbol::var:
		{
			int lineNmbr = m_current_line;
			next();
			std::string variableID = m_current_token.lexeme;
			match(Symbol::identifier, "<identifier>");
			match(Symbol::colon, "\":\"");
			ValueType type = varType();
			Expression* expression = null;
			if(m_current_token.symbol == Symbol::op_assign) {
				next();
				expression = expr();
			}
			Statement* stmnt = new VarDeclaration(variableID, expression, type);
			stmnt->type = StatementType::declaration;
			stmnt->lineNumber = lineNmbr;
			return stmnt;
		}
	case Symbol::identifier:
		{
			std::string idName = m_current_token.lexeme;
			next();
			match(Symbol::op_assign, ":=");
			Expression* ex = expr();
			Statement* stmnt = new AssignStatement(idName, ex);
			stmnt->type = StatementType::assign;
			stmnt->lineNumber = ex->lineNumber;
			return stmnt;
		}
	case Symbol::for_stmt:
		{
			int lineNmbr = m_current_line;
			next();
			std::string varName = m_current_token.lexeme;
			match(Symbol::identifier, "<identifier>");
			match(Symbol::in, "in");
			Expression* from = expr();
			match(Symbol::range, "..");
			Expression* to = expr();
			match(Symbol::do_stmt, "do");
			Statement* statements = stmnts();
			match(Symbol::end, "end");
			match(Symbol::for_stmt, "for");
			Statement* stmnt = new ForStatement(varName, from, to, statements);
			stmnt->type = StatementType::for_stmnt;
			stmnt->lineNumber = lineNmbr;
			return stmnt;
		}
	case Symbol::read:
		{
			int lineNmbr = m_current_line;
			next();
			std::string destVar = m_current_token.lexeme;
			match(Symbol::identifier, "<identifier>");
			Statement* stmnt = new ReadStatement(destVar);
			stmnt->type = StatementType::read;
			stmnt->lineNumber = lineNmbr;
			return stmnt;
		}
	case Symbol::print:
		{
			next();
			Expression* ex = expr();
			Statement* stmnt = new PrintStatement(ex);
			stmnt->type = StatementType::print;
			stmnt->lineNumber = ex->lineNumber;
			return stmnt;
		}
	case Symbol::assert:
		{
			next();
			match(Symbol::lparen, "(");
			Expression* ex = expr();
			match(Symbol::rparen, ")");
			Statement* stmnt = new AssertStatement(ex);
			stmnt->type = StatementType::assert;
			stmnt->lineNumber = ex->lineNumber;
			return stmnt;
		}
	default:
		//std::cout << "Syntax error! Unexpected token at " << m_current_line << ":" << m_current_index << "." << std::endl;
		std::ostringstream oss;
		oss << "Syntax error! Unexpected token at " << m_current_line << ":" << m_current_index << "." << std::endl;
		throw std::logic_error( oss.str() );
		return null;
	}
};

Expression* Parser::expr()
{
	Expression* node = null;
	bool notPresent = false;
	if(m_current_token.symbol == Symbol::op_not)
	{
		notPresent = true;
		next();
	}

	Expression* node1 = opnd();

	if(is_operator()){
		OperatorType type = op();
		Expression* node2 = opnd();
		node = new BinaryOp(type, node1, node2);
	} else {
		Expression* ex;
		if(notPresent)
		{
			ex = new UnaryOp(UnaryOperandType::not, node1);
			ex->exprType = ExpressionType::operation;
			node = ex;
		} else {
			ex = new UnaryOp(node1);
			ex->exprType = ExpressionType::unary;
			node = ex;
		};
	};
	node->lineNumber = m_current_line;
	node->column = node1->column;
	return node;
};

bool Parser::is_operator()
{
	switch(m_current_token.symbol)
	{
	case Symbol::op_plus :return true;
	case Symbol::op_minus:return true;
	case Symbol::op_mult: return true;
	case  Symbol::op_div: return true;
	case Symbol::op_less: return true;
	case Symbol::op_equal: return true;
	case Symbol::op_and: return true;
	case Symbol::op_not: return true;
	default: return false;
	};
};

Expression* Parser::opnd()
{
	Expression* node = null;
	switch(m_current_token.symbol)
	{
	case Symbol::integer:
		{
			node = new IntegerConst(m_current_token.lexeme);
			node->lineNumber = m_current_line;
			node->column = m_current_index - m_current_token.lexeme.length();
			next();
			break;
		}
	case Symbol::string_literal:
		{
			node = new StringLiteral(m_current_token.lexeme);
			node->lineNumber = m_current_line;
			node->column = m_current_index - m_current_token.lexeme.length();
			next();
			break;
		}
	case Symbol::identifier:
		{
			node = new Variable(m_current_token.lexeme);
			node->lineNumber = m_current_line;
			node->column = m_current_index - m_current_token.lexeme.length();
			next();
			break;
		}
	case Symbol::lparen:
		next();
		node = expr();
		match(Symbol::rparen, ")");
		break;
	default:
		std::ostringstream oss;
		oss << "Unexpected operand at " << m_current_line << ":" << m_current_index - m_current_token.lexeme.length() << "." << std::endl;
		throw std::logic_error( oss.str() );
	};

	return node;
};

OperatorType Parser::op()
{
	switch(m_current_token.symbol)
	{
	case Symbol::op_plus :
		next();
		return OperatorType::plus;
	case Symbol::op_minus:
		next();
		return OperatorType::minus;
	case Symbol::op_mult: 
		next();
		return OperatorType::multiplication;
	case  Symbol::op_div:
		next();
		return OperatorType::division;
	case Symbol::op_less:
		next();
		return OperatorType::less;
	case Symbol::op_equal:
		next();
		return OperatorType::equal;
	case Symbol::op_and:
		next();
		return OperatorType::and;
	case Symbol::op_not:
		next();
		return OperatorType::not;
	default:
		std::ostringstream oss;
		oss << "Unexpected operator at " << m_current_line << ":" << m_current_index - m_current_token.lexeme.length() << "." << std::endl;
		throw std::logic_error( oss.str() );
		//return OperatorType::undefined;
	};
}

ValueType Parser::varType()
{
	switch(m_current_token.symbol)
	{
	case Symbol::integer:
		next();
		return ValueType::number;
	case Symbol::string:
		next();
		return ValueType::string_literal;
	case Symbol::boolean:
		next();
		return ValueType::boolean;
	default:
		std::ostringstream oss;
		oss << "Unexpected type at " << m_current_line << ":" << m_current_index - m_current_token.lexeme.length() << "." << std::endl;
		throw std::logic_error( oss.str() );
		//return ValueType::undefined;
	};
};

bool Parser::match(Symbol symbol, std::string tag)
{
	if(m_current_token.symbol == symbol)
	{
		next();
		return true;
	} 
	std::ostringstream oss;
	oss << "Unexpected token at "  << m_current_line << ":" << m_current_index - m_current_token.lexeme.length() <<  ", expected token " << tag << "." << std::endl;
	throw std::logic_error( oss.str() );
	return false;
};

void Parser::handleError()
{

	error = true;
	bool safe = false;

	while(!safe) {
		next();
		switch(m_current_token.symbol)
		{
		case ( Symbol::var):
			safe = true;
			break;
		case ( Symbol::for_stmt):
			safe = true;
			break;
		case ( Symbol::print ):
			safe = true;
			break;
		case ( Symbol::read ):
			safe = true;
			break;
		case ( Symbol::assert ):
			safe = true;
			break;
		case ( Symbol::end_of_text ):
			safe = true;
			break;
		default:
			safe = false;
			break;
		};
	}
};

void Parser::next()
{
	m_current_token = m_scanner.nextToken();

	// Skips whitespace, newlines and comments but keeps track of the current line and column.
	while(m_current_token.symbol == Symbol::whitespace || m_current_token.symbol == Symbol::newline ||  m_current_token.symbol == Symbol::comment)
	{
		if(m_current_token.symbol == Symbol::whitespace)
		{
			m_current_index++;
		}
		// Skips comments but saves the info of the skipped lines and indexes.
		if(m_current_token.symbol == Symbol::comment)
		{
			m_current_index += m_current_token.lexeme.length();
			// Check how many newlines are contained in the comment.
			for(unsigned i=0; i<m_current_token.lexeme.length(); i++)
			{
				if(m_current_token.lexeme.at(i) == '\n')
				{
					m_current_line++;
					m_current_index = 1;
				};
			}
		};
		if(m_current_token.symbol == Symbol::newline)
		{
			m_current_line++;
			m_current_index = 1;
		};
		m_current_token = m_scanner.nextToken();
	}

	if(m_current_token.symbol == Symbol::string_literal) 
	{
		m_current_token.lexeme = fixLexeme(m_current_token.lexeme);
	};

	m_current_index += m_current_token.lexeme.length();
};

/*
This function strips away the " characters from the beginning and the end of the string.
Also all escape sequences are now in a form "\n", consisting of the chars '\\' and 'n'
so the form "\n" has to be changed to '\n'.
*/
std::string Parser::fixLexeme(std::string lexeme)
{
	std::string lex = lexeme.substr(1, lexeme.length() - 2);
	std::string retLexeme;
	for(unsigned i=0; i<lex.length(); i++)
	{
		if(lex.at(i) == '\\')
		{
			char nextChar = lex.at(i+1); // There has to be one more because can't end in '\'
			switch(nextChar)
			{
			case 'a':
				retLexeme += '\a';
				break;
			case 'b':
				retLexeme += '\b';
				break;
			case 't':
				retLexeme += '\t';
				break;
			case 'n':
				retLexeme += '\n';
				break;
			case 'v':
				retLexeme += '\v';
				break;
			case 'f':
				retLexeme += '\f';
				break;
			case 'r':
				retLexeme += '\r';
				break;
			case '"':
				retLexeme += '"';
				break;
			case '\'':
				retLexeme += '\'';
				break;
			case '?':
				retLexeme += '\?';
				break;
			case '\\':
				retLexeme += '\\';
				break;
			};
			i++;
		} else {
			retLexeme += lex.at(i);
		};
	};
	return retLexeme;
};