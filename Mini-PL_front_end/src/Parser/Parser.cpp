#include "Parser/Parser.h"
//#include "Parser/Nodes/Operand.h"
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

Parser::Parser(Scanner& scanner) 
	: m_scanner(scanner), m_status(Status::no_error), m_current_line(1), m_current_index(1) {};

Statement* Parser::parse()
{
	m_scanner.scan();

	return prog();
};

Statement* Parser::prog()
{
	next();
	return stmnts();
}

Statement* Parser::stmnts()
{
	Statement* statement = stmnt();
	match(Symbol::semicolon, ";");
	statement->next = stmnts_tail();
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
			std::cout << m_current_token.lexeme;
			next();
			std::string variableID = m_current_token.lexeme;
			match(Symbol::identifier, "<identifier>");
			match(Symbol::colon, "\":\"");
			ValueType type = varType();
			Expression* expression = null;
			if(m_current_token.symbol == Symbol::op_assign) {
				std::cout << m_current_token.lexeme;
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
			std::cout << m_current_token.lexeme;
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
			std::cout << m_current_token.lexeme;
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
			std::cout << m_current_token.lexeme;
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
			std::cout << m_current_token.lexeme;
			next();
			Expression* ex = expr();
			Statement* stmnt = new PrintStatement(ex);
			stmnt->type = StatementType::print;
			stmnt->lineNumber = ex->lineNumber;
			return stmnt;
		}
	case Symbol::assert:
		{
			std::cout << m_current_token.lexeme;
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
		std::cout << "Syntax error! Unexpected token at " << m_current_line << ":" << m_current_index << "." << std::endl;
		next();
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
		std::cout << m_current_token.lexeme;
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
			std::cout << m_current_token.lexeme;
			node = new IntegerConst(m_current_token.lexeme);
			node->lineNumber = m_current_line;
			node->column = m_current_index - m_current_token.lexeme.length();
			next();
			break;
		}
	case Symbol::string:
		{
			std::cout << m_current_token.lexeme;
			node = new StringLiteral(m_current_token.lexeme);
			node->lineNumber = m_current_line;
			node->column = m_current_index - m_current_token.lexeme.length();
			next();
			break;
		}
	case Symbol::identifier:
		{
			std::cout << m_current_token.lexeme;
			node = new Variable(m_current_token.lexeme);
			node->lineNumber = m_current_line;
			node->column = m_current_index - m_current_token.lexeme.length();
			next();
			break;
		}
	case Symbol::lparen:
		std::cout << m_current_token.lexeme;
		next();
		node = expr();
		match(Symbol::rparen, ")");
		break;
	default:
		std::cout << "Unexpected operand at " << m_current_line << ":" << m_current_index - m_current_token.lexeme.length() << "." << std::endl;
		next();
	};

	return node;
};

OperatorType Parser::op()
{
	switch(m_current_token.symbol)
	{
	case Symbol::op_plus :
		std::cout << m_current_token.lexeme;
		next();
		return OperatorType::plus;
	case Symbol::op_minus:
		std::cout << m_current_token.lexeme;
		next();
		return OperatorType::minus;
	case Symbol::op_mult: 
		std::cout << m_current_token.lexeme;
		next();
		return OperatorType::multiplication;
	case  Symbol::op_div:
		std::cout << m_current_token.lexeme;
		next();
		return OperatorType::division;
	case Symbol::op_less:
		std::cout << m_current_token.lexeme;
		next();
		return OperatorType::less;
	case Symbol::op_equal:
		std::cout << m_current_token.lexeme;
		next();
		return OperatorType::equal;
	case Symbol::op_and:
		std::cout << m_current_token.lexeme;
		next();
		return OperatorType::and;
	case Symbol::op_not:
		std::cout << m_current_token.lexeme;
		next();
		return OperatorType::not;
	default:
		std::cout << "Unexpected operator at " << m_current_line << ":" << m_current_index - m_current_token.lexeme.length() << "." << std::endl;
		return OperatorType::undefined;
	};
}

ValueType Parser::varType()
{
	switch(m_current_token.symbol)
	{
	case Symbol::integer:
		std::cout << m_current_token.lexeme;
		next();
		return ValueType::number;
	case Symbol::string:
		std::cout << m_current_token.lexeme;
		next();
		return ValueType::string;
	case Symbol::boolean:
		std::cout << m_current_token.lexeme;
		next();
		return ValueType::boolean;
	default:
		std::cout << "Unexpected type at " << m_current_line << ":" << m_current_index - m_current_token.lexeme.length() << "." << std::endl;
		m_status = Status::type_error;
		return ValueType::undefined;
	};
};

bool Parser::match(Symbol symbol, std::string tag)
{
	if(m_current_token.symbol == symbol)
	{
		std::cout << m_current_token.lexeme;
		next();
		return true;
	} 
	std::cout << "Unexpected token at "  << m_current_line << ":" << m_current_index - m_current_token.lexeme.length() <<  ", expected token " << tag << "." << std::endl;
	m_status = Status::syntax_error;
	return false;
};

void Parser::handleError()
{

};

void Parser::next()
{
	m_current_token = m_scanner.nextToken();

	// Skips whitespace, newlines and comments but keeps track of the current line and column.
	while(m_current_token.symbol == Symbol::whitespace || m_current_token.symbol == Symbol::newline ||  m_current_token.symbol == Symbol::comment)
	{
		if(m_current_token.symbol == Symbol::whitespace)
		{
			std::cout << m_current_token.lexeme;
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
			std::cout << m_current_token.lexeme;
			m_current_line++;
			m_current_index = 1;
		};
		m_current_token = m_scanner.nextToken();
	}

	if(m_current_token.symbol == Symbol::string) 
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