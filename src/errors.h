#ifndef ERRORS_H
#define ERRORS_H

#include <optional>
#include <string>
#include <vector>
#include <variant>

namespace Errors
{
	enum TokenErrorCode { E0001 = 1 };
	enum ParserErrorCode { E0101 = 101, E0102, E0103, E0104, E0105, E0106, E0107, E0108, E0109, E0110};
}

struct Error
{
	std::variant <Errors::TokenErrorCode, Errors::ParserErrorCode> errorCode;
	std::string errorMessage;
	std::pair<int, int> coord;
	std::string fileName;
	std::optional<char> value {};
	void raise();
	int errorCodeValue;
};

#endif