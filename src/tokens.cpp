#include "tokens.h"

// need to divide up into different operatosr
//! assignment
//!		=
//! Arithmetic
//!		+, -, *, /, %
//! Compound
//!		likely need to create this as an addition to the other operators
//!		+=, -=, *=, /=, %=, >>=, <<=, &=, ^=, |=
//! Increment/Decrement
//!		++, --
//! Relational/Comparison Operators
//!		==, !=, <, >, >=, <=
//! Logical Operators
//!		!, &&, ||
//! Conditional Ternary Operator
//!		?
//! Comma operator
//!		,
//! Bitwise Operator
//!		&, |, ^, ~, <<, >>
//! Explicit typecasting operator	
//!		(typename) foo, typname (foo)
//! Precedence operators (order of operations)
//!		(, )

std::vector<TokenType> OperatorGroups::OperatorTokens = {TokenType::ADDITION, TokenType::SUBTRACTION, TokenType::MULTIPLICATION, TokenType::DIVISION};
std::vector<std::string> OperatorGroups::OperatorStrings = { "+", "-", "*", "/"};
std::vector<TokenType> OperatorGroups::UnaryOperators = { TokenType::INCREMENT, TokenType::DECREMENT };
std::vector<std::string> OperatorGroups::UnaryOperatorStrings = { "++", "--"};