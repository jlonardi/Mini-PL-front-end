#pragma once

#ifndef RULES_H
#define RULES_H

#include <vector>
#include <map>

/*
	In each table works the following principle. The column 'j' rapresents
	the input read and each row 'i' rapresents one state. So for example
	in the "state_transitions" table state_transitions[i][j] tells in
	what state you will be moving on the given input. 
	
	The "actions" table tells what we are supposed to do (what is our action) 
	on the given input, if the input is a valid move to next state, if it's 
	not then accept this state (and buffer the invalid input).

	The "state_labels" simply stores the information of the label of the state.
	For example lets say we are at the state that acceptes when token is an 
	"int" (state 2). If we read something else than a digit it will mean
	that we are going to accept the state 2 and buffer the read char. The
	value at the position [i][j] will contain an index for the "labels" 
	array where the correct label/tag for the token will be found.

	The "input_column" maps the input character to a column index. This
	column index is the index used in the "state_transitions",
	"state_actions" and "state_labels" tables. So that each column index
	will be rapresenting an input character.
*/
struct RuleSet {
	std::vector<std::string> labels;
	std::vector<std::string> reserved_words;
	std::vector<std::vector<int>> state_transitions;
	std::vector<std::vector<int>> state_actions;
	std::vector<std::vector<int>> state_labels;
};

struct Token
{
	//std::string tag;
	std::string lexeme;
	enum Symbol symbol;
};

/*
	A generic interface for the rules that describes a regular language.
	The rules are used in a table-driven automaton that is used by the
	Scanner to tokenize the source code.
*/
class Rules
{
public:
	virtual ~Rules() { };
	virtual RuleSet getRules() = 0;
	virtual int characterCategory(char c) = 0;
	virtual Symbol generateSymbol(std::string tag) = 0;
};

#endif