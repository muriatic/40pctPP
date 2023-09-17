#include "tokens.h"

std::vector<TokenType> OperatorGroups::OperatorTokens = {TokenType::ADDITION, TokenType::SUBTRACTION, TokenType::MULTIPLICATION, TokenType::DIVISION};
std::vector<std::string> OperatorGroups::OperatorStrings = { "+", "-", "*", "/"};
std::vector<TokenType> OperatorGroups::UnaryOperators = { TokenType::INCREMENT, TokenType::DECREMENT };
std::vector<std::string> OperatorGroups::UnaryOperatorStrings = { "++", "--"};