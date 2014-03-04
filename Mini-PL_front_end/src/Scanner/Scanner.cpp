#include "Scanner/Scanner.h"

#include <iostream>

Scanner::Scanner(Buffer& buffer, Rules& rules) 
	: m_buffer(buffer), m_rules(rules) { }

void Scanner::scan()
{
	RuleSet ruleSet = m_rules.getRules();
	int state = 0;	// initial state
	std::string lexeme = "";

	while(!m_buffer.empty())
	{
		char c = m_buffer.read();
		int input = m_rules.characterCategory(c);
		// State action with the input is MOVE and the state where to move from this state with the given input is not -1.
		if(ruleSet.state_actions[state][input] == 1 && ruleSet.state_transitions[state][input] != -1)
		{
			lexeme += c;
			state = ruleSet.state_transitions[state][input];
		} 
		// If cant move to any state with the given input and the action is accept create a token and add it to the token list.
		else if(ruleSet.state_transitions[state][input] == -1 && ruleSet.state_actions[state][input] == 2)
		{
			m_buffer.push(c); // push the input back to the buffer
			Token tkn;
			tkn.lexeme = lexeme;
			int label_index = ruleSet.state_labels[state][input];	// get the index of the label corresponding this state
			std::string tag = ruleSet.labels[label_index];
			// check if the lexeme is a keyword
			for(unsigned i=0; i<ruleSet.reserved_words.size(); i++)
			{
				if(!tkn.lexeme.compare(ruleSet.reserved_words[i]))
				{
					tag = ruleSet.reserved_words[i];
				}
			}

			tkn.symbol = m_rules.generateSymbol(tag);
			m_tokens.push_back(tkn); // if token is not whitespace or comment save it

			lexeme = ""; // reset the lexeme
			state = 0; // reset the state
		} 
		else { // The action of the state has to be ERROR, create an error token
			Token tkn;
			tkn.lexeme = "ERROR";
			tkn.symbol = m_rules.generateSymbol("ERROR");
			m_tokens.push_back(tkn);
			lexeme = ""; // reset the lexeme
			state = 0; // reset the state
		}

	}

	Token tkn;
	tkn.symbol = m_rules.generateSymbol("END_OF_TEXT");
	tkn.lexeme = "END";
	m_tokens.push_back(tkn);
}

Token Scanner::nextToken()
{
	Token tkn = m_tokens.front();
	m_tokens.pop_front();
	return tkn;
}

bool Scanner::tokensLeft()
{
	return !m_tokens.empty();
}