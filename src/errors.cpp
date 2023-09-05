#include <string>
#include <iostream>
#include <format>
#include <vector>
#include <optional>

#include "errors.h"


void Error::raise()
{
	struct ErrorCodeVisitor
	{
		Error* error;
		void operator()(const Errors::TokenErrorCode& tokenErrorCode) const
		{
			error->errorCodeValue = tokenErrorCode;
		}
		void operator()(const Errors::ParserErrorCode& parserErrorCode) const
		{
			error->errorCodeValue = parserErrorCode;
		}
	};

	ErrorCodeVisitor visitor{ .error = this };
	std::visit(visitor, errorCode);

	std::string errorCodeString = "Error " + std::format("{:04d}", errorCodeValue);


	std::string valueSection = "";
	char quoteCharacter = '\'';
	if (value.has_value())
	{
		char val = *value;
		if (val == '\'')
		{
			quoteCharacter = '\"';
		}

		// apologies for this weirdness but if i do it in one line it gets weird
		valueSection += ' ';
		valueSection += quoteCharacter;
		valueSection += val;
		valueSection += quoteCharacter;
	}
	
	std::string locationInfo = std::to_string(coord.first) + ":" + std::to_string(coord.second);
	std::string m_errorMessage = fileName + ":" + locationInfo + ": " + errorCodeString +": " + errorMessage + "" + valueSection;
	std::cerr << m_errorMessage;
	
	exit(EXIT_FAILURE);
}
