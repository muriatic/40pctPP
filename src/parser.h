#ifndef PARSER_H
#define PARSER_H

#include <variant>
#include <optional>
#include <vector>


#include "tokens.h"
#include "position.h"


struct NodeExprIntLit {
	Tokens int_lit;
};

//struct OperatorExpression {
//	std::variant<NodeExprIntLit, NodeExprIdent> var;
//};

struct NodeExprOperator
{
	//
	Tokens Operation;
};

struct NodeExprUnaryOperator
{
	//
	Tokens Operation;
};

struct NodeExprIdent {
	Tokens ident;
};

struct NodeExpr {
	std::vector<std::variant<NodeExpr, NodeExprIntLit, NodeExprIdent, NodeExprOperator, NodeExprUnaryOperator>> var;
	//std::variant<NodeExprIntLit, NodeExprIdent> var;
};

struct NodeStmtExit {
	
};

struct NodeStmtReturn {
	NodeExpr expr;
};

struct NodeStmtIntDef {
	Tokens IDENT;
	NodeExpr expr;
};

struct NodeStmtIntAssignment {
	Tokens IDENT;
	NodeExpr expr;
};

struct NodeStmtIntOperation {
	Tokens IDENT;
	NodeExpr expr;
};

struct NodeStmt {
	std::variant<NodeStmtReturn, NodeStmtExit, NodeStmtIntDef, NodeStmtIntAssignment, NodeStmtIntOperation> var;
};

struct NodeProg {
	std::vector<NodeStmt> stmts;
};


class Parser
{
	std::vector <Tokens> m_tokens;
	size_t m_index = 0;
public:
	Parser(std::vector <Tokens> tokens);
	std::optional<NodeExpr> ParseExpr();
	std::optional<NodeStmt> ParseStmt();
	std::optional<NodeProg> ParseProgram();
private:
	std::optional <Tokens> Peek(int offset = 0) const;
	Tokens Consume();
	bool CheckParentheses();
	std::vector <std::string> m_idents;
	Position position{ 1, 1 };
	std::string m_fileName;
	int parentheses = 0;
};

#endif