#ifndef TOKENS_H
#define TOKENS_H

#include <optional>
#include <string>
#include <vector>


#include "position.h"
//#pragma once

enum TokenType { 
	RETURN, INTEGER_DEF, OPEN_PAREN, CLOSE_PAREN, INT_LITERAL, SEMICOLON, IDENT, EQUALS, ADDITION, EXIT, 
	SUBTRACTION, MULTIPLICATION, DIVISION, INCREMENT, DECREMENT, IF, OPEN_BRACKET, CLOSE_BRACKET, IS_EQUAL_TO,
	MODULO, GREATER_THAN, LESS_THAN, NOT_EQUAL_TO, GREATER_THAN_OR_EQUAL, LESS_THAN_OR_EQUAL
};


struct Tokens
{
	TokenType type;

	// (line number, column number)
	Position position;
	std::optional<std::string> value {};
};

namespace OperatorGroups
{
	extern std::vector<TokenType> OperatorTokens;
	extern std::vector<TokenType> UnaryOperators;
	extern std::vector<std::string> UnaryOperatorStrings;
	extern std::vector<std::string> OperatorStrings;
}

//struct TokenGroups
//{
//};

///*, TokenType::SUBTRACTION, TokenType::MULTIPLICATION, TokenType::DIVISION*/
//extern std::vector<TokenType> OperatorTokens;

#endif