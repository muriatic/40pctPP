#include <vector>
#include <string>

#include "tokens.h"
#include "vector_functions.h"

//struct Operators
//{
//	std::string Get_c_Representation(TokenType token)
//	{
//		int idx = IsIn(tokens, token);
//		return c_representation.at(idx);
//	}
//
//	std::string Get_local_Representation(TokenType token)
//	{
//		int idx = IsIn(tokens, token);
//		return local_representation.at(idx);
//	}
//
//	// maybe add get token function
//
//	/*std::vector<TokenType> static getTokens()
//	{
//		return tokens;
//	}*/
//
//	std::vector<TokenType> static tokens;
//protected:
//	std::vector<std::string> c_representation;
//	std::vector<std::string> local_representation;
//};

namespace ArithmeticOperators
{
	std::vector<TokenType> static tokens = { TokenType::ADDITION, TokenType::SUBTRACTION, TokenType::MULTIPLICATION, TokenType::DIVISION, TokenType::MODULO };
	std::vector<std::string> c_representation = { "+", "-", "*", "/", "%" };
	std::vector<std::string> local_representation = { "+", "-", "*", "/", "%" };
};

namespace UnaryOperators
{
	std::vector<TokenType> static tokens = { TokenType::INCREMENT, TokenType::DECREMENT };
	std::vector<std::string> c_representation = { "++", "--" };
	std::vector<std::string> local_representation = { "++", "--" };
};

namespace AssignmentOperators
{
	std::vector<TokenType> static tokens = { TokenType::EQUALS };
	std::vector<std::string> c_representation = { "=" };
	std::vector<std::string> local_representation = { "=" };
};

namespace ComparisonOperators
{
	std::vector<TokenType> static tokens = { TokenType::IS_EQUAL_TO, TokenType::GREATER_THAN, TokenType::LESS_THAN, TokenType::NOT_EQUAL_TO, TokenType::GREATER_THAN_OR_EQUAL, TokenType::LESS_THAN_OR_EQUAL };
	std::vector<std::string> c_representation = { "==", ">", "<", "!=", ">=", "<=" };
	std::vector<std::string> local_representation = { "==", ">", "<", "!=", ">=", "<=" };
};