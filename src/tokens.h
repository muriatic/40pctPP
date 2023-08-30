#include <optional>
#include <string>

#pragma once

enum TokenType { RETURN, INTEGER_DEF, OPEN_PAREN, CLOSE_PAREN, INT_LITERAL, SEMICOLON, IDENT, EQUALS, ADDITION/*, EXIT*/ };


struct Tokens
{
	TokenType type;
	std::optional<std::string> value {};
};
