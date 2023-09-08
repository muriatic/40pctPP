#ifndef ERRORS_H
#define ERRORS_H

#include <optional>
#include <string>
#include <vector>
#include <variant>


// remove before production
const std::string str_types[] = { "RETURN", "INTEGER_DEF", "open_paren", "close_paren", "int_literal", "semicolon" , "ident", "equals", "ADDITION", "EXIT" };


struct Errors
{
    Errors(std::string val = "")
    {
        UpdateQuoteCharacter(val);
    }

    void Raise()
    {
        std::string message = fileName + ":" + m_coord.getColonRepresentation() + ": " + errorCode + ": " + m_errorMessage;
        std::cerr << message << std::endl;
        exit(EXIT_FAILURE);
    }

    void UpdateQuoteCharacter(std::string value)
    {
        quoteCharacter = "\'";

        if (value == quoteCharacter)
        {
            quoteCharacter = "\"";
        }
    }
    static std::string fileName;
    Position m_coord;
    std::string m_errorMessage;
    std::string errorCode;
    std::string quoteCharacter;
};


struct E0001 : Errors
{
    E0001(std::string val, Position coord, std::string customErrorMessage = "")
        : Errors(val)
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage == "")
        {
            m_errorMessage = "Unrecognized character " + quoteCharacter + val + quoteCharacter;
        }
    }
};

struct E0101 : Errors
{
    E0101(Position coord, std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage == "")
        {
            m_errorMessage = "Expected Expression";
        }
    }
};

struct E0102 : Errors
{
    E0102(Position coord, std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage == "")
        {
            m_errorMessage = "Expected '('";
        }
    }
};

struct E0103 : Errors
{
    E0103(Position coord, std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage == "")
        {
            m_errorMessage = "Expected ')'";
        }
    }
};

struct E0104 : Errors
{
    E0104(Position coord, std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage == "")
        {
            m_errorMessage = "Expected ';'";
        }
    }
};

// Invalid Integer Definition
struct E0105 : Errors
{
    enum ErrorTypes {ALREADY_DEFINED, NO_EQUALS, NO_VAR_NAME, EXPECT_INT_LITERAL, EXPECT_SEMICOLON};

    E0105(Position coord, ErrorTypes errorType, std::string val = "", std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage != "")
        {
            return; 
        }

        if (errorType == ErrorTypes::NO_VAR_NAME)
        {
            m_errorMessage = "Invalid integer definition, expected an variable name after INT.";
        }
        else if (errorType == ErrorTypes::ALREADY_DEFINED)
        {
            m_errorMessage = "Invalid integer definition, variable " + val + " has already been initialized and defined";
        }
        else if (errorType == ErrorTypes::NO_EQUALS)
        {
            m_errorMessage = "Invalid integer definition, expected an '=' after " + val;
        }
        else if (errorType == ErrorTypes::EXPECT_INT_LITERAL)
        {
            m_errorMessage = "Invalid integer definition, expected integer literal or variable.";
        }
        else if (errorType == ErrorTypes::EXPECT_SEMICOLON)
        {
            m_errorMessage = "Invalid integer definition, expected ';'";
        }
    }
};

// Invalid integer assignment
struct E0106 : Errors
{
    enum ErrorTypes {NO_IDENT, VAR_NOT_INITIALIZED, NO_EQUAL, EXPECTED_INT_LITERAL, EXPECTED_SEMICOLON};

    E0106(Position coord, ErrorTypes errorType, std::string val = "", std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage != "")
        {
            return;
        }

        if (errorType == ErrorTypes::NO_IDENT)
        {
            m_errorMessage = "Invalid integer assignment, IDENT token has no value";
        }

        else if (errorType == ErrorTypes::VAR_NOT_INITIALIZED)
        {
            m_errorMessage = "Invalid integer assignment, variable " + val + " has not been initialized and/or defined. \nPlease define it like `INT " + val + " = ...;`";
        }

        else if (errorType == ErrorTypes::NO_EQUAL)
        {
            m_errorMessage = "Invalid integer assignment, expected an `=` after " + val;
        }
        
        else if (errorType == ErrorTypes::EXPECTED_INT_LITERAL)
        {
            m_errorMessage = "Invalid integer assignment, expected integer literal or variable.";
        }
        
        else if (errorType == ErrorTypes::EXPECTED_SEMICOLON)
        {
            m_errorMessage = "Invalid integer assignment, expected ';'";
        }
    }
};

// invalid exit statement
struct E0107 : Errors
{
    E0107(Position coord, std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage == "")
        {
            m_errorMessage = "Invalid function termination, expected ';'";
        }
    }
};

// Invalid Statement
struct E0108 : Errors
{
    E0108(Position coord, std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage == "")
        {
            m_errorMessage = "Invalid statement";
        }
    }
};

// unrecognized Character
struct E0109 : Errors
{
    E0109(Position coord, std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage == "")
        {
            m_errorMessage = "Unrecognized Character";
        }
    }
};

// Invalid token or order
struct E0201 : Errors
{
    E0201(Position coord, std::string val, std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage == "")
        {
            m_errorMessage = "Invalid token of type: " + val + " or order. ";
        }
    }
};

// variable has already been initialized or defined
struct E0202 : Errors
{
    E0202(Position coord, std::string val, std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage == "")
        {
            m_errorMessage = "Variable " + val + " has already been initialized and defined";
        }
    }
};

// variable has not already been initialized or defined
struct E0203 : Errors
{
    E0203(Position coord, std::string val, std::string customErrorMessage = "")
        : Errors()
    {
        m_coord = coord;
        errorCode = __func__;

        m_errorMessage = customErrorMessage;
        if (customErrorMessage == "")
        {
            m_errorMessage = "Variable " + val + " has not already been initialized and defined";
        }
    }
};


#endif