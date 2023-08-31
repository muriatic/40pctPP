#include "tokens.h"

std::vector<TokenType> OperatorGroups::OperatorTokens = {TokenType::ADDITION, TokenType::SUBTRACTION, TokenType::MULTIPLICATION, TokenType::DIVISION};
std::vector<std::string> OperatorGroups::OperatorStrings = { "+", "-", "*", "/"};