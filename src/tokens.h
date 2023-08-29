#include <optional>
#include <string>

#pragma once

enum TokenType { RETURN, OPEN_PAREN, CLOSE_PAREN, INT_LITERAL, SEMICOLON, IDENT, EQUALS/*, EXIT*/ };


struct Tokens
{
	TokenType type;
	std::optional<std::string> value {};
};
