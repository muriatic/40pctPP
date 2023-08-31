#ifndef TOKENS_H
#define TOKENS_H

#include <optional>
#include <string>
#include <vector>

//#pragma once

enum TokenType { RETURN, INTEGER_DEF, OPEN_PAREN, CLOSE_PAREN, INT_LITERAL, SEMICOLON, IDENT, EQUALS, ADDITION/*, EXIT*/ };


struct Tokens
{
	TokenType type;
	std::optional<std::string> value {};
};

namespace OperatorGroups
{
	extern std::vector<TokenType> OperatorTokens;
	extern std::vector<std::string> OperatorStrings;
}

//struct TokenGroups
//{
//};

///*, TokenType::SUBTRACTION, TokenType::MULTIPLICATION, TokenType::DIVISION*/
//extern std::vector<TokenType> OperatorTokens;

#endif