#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "parser.h"
#include "tokens.h"
#include "errors.h"

class Generator {
public:
	Generator(NodeProg prog)
		:m_prog(std::move(prog))
	{}

	void GenExpr(const NodeExpr& expr)
	{
		struct ExprVisitor {
			Generator* gen;
			// if the expression has an integer literal
			void operator()(const NodeExprIntLit& expr_int_literal) const
			{
				// point to m_output << "return" << within the int literal expression, access the integer literal's value.value()
				//std::cout << "test" << std::endl;
				gen->m_output << expr_int_literal.int_lit.value.value()/* << ";\n"*/;
			}
			void operator()(const NodeExprIdent& expr_ident) const
			{
				//std::cout << "int " << gen->m_vars.at(expr_ident.ident.value.value()) << std::endl;

				//std::cout << "expr_ident.ident.value.value()" << expr_ident.ident.value.value() << std::endl;
				// define integer 
				//gen->m_output << "int " << expr_ident.ident.value.value() << "=";
				gen->m_output << expr_ident.ident.value.value();
			}
			void operator()(const NodeExprChain& expr_chain) const
			{
				std::vector<TokenType> edgeTokens { TokenType::IDENT, TokenType::INT_LITERAL };
				//// the first and last token must be either an INT_LITERAL or IDENT
				////! this should be moved to parsing but ehh 
				//if (std::find(edgeTokens.begin(), edgeTokens.end(), expr_chain.tokens[0]) == edgeTokens.end() ||
				//	std::find(edgeTokens.begin(), edgeTokens.end(), expr_chain.tokens.back()) == edgeTokens.end())
				//{
				//	std::cerr << "Chained expressions must start and end with either identifiers or integer literals" << std::endl;
				//	exit(EXIT_FAILURE);
				//}

				// kinda cheating but works
				bool isEven = true;
				for (int i = 0; i < expr_chain.tokens.size(); i++)
				{

					// if its an INT literal or ident and supposed to be
					if (isEven &&
						std::find(edgeTokens.begin(), edgeTokens.end(), expr_chain.tokens[i].type) != edgeTokens.end())
					{
						gen->m_output << " " << expr_chain.tokens[i].value.value();
					}

					else if (!isEven &&
						std::find(OperatorGroups::OperatorTokens.begin(), OperatorGroups::OperatorTokens.end(), expr_chain.tokens[i].type) != OperatorGroups::OperatorTokens.end())
					{
						// find position of operator in operator groups
						auto it = find(OperatorGroups::OperatorTokens.begin(), OperatorGroups::OperatorTokens.end(), expr_chain.tokens[i].type);

						int idx = it - OperatorGroups::OperatorTokens.begin();

						gen->m_output << " " << OperatorGroups::OperatorStrings[idx];
					}

					else {
						E0201 error(expr_chain.tokens[i].position, str_types[expr_chain.tokens[i].type]);
						error.Raise();
					}
					isEven = !isEven;
				}
			}
		};

		// send in the value of gen as the current instance of this class
		ExprVisitor visitor { .gen = this };

		// visit based on which type of expr this is
		std::visit(visitor, expr.var);
	}

	void GenStmt(const NodeStmt& stmt)
	{
		// stmt is a variant coming in

		// looks a little cursed but this is the function that we will visit from inside to deal with out NodeStmt and it's type
		struct StmtVisitor {
			// create pointer to parent class
			Generator* gen;
			// operator when we get a NodeStmtReturn&
			void operator()(const NodeStmtReturn& stmt_return) const
			{
				gen->m_output << "return ";
				// access GenExpr from parent class
				gen->GenExpr(stmt_return.expr);

				gen->m_output << ";\n";
			}
			void operator()(const NodeStmtIntDef& stmt_INT_def) const
			{
				//gen->m_output << "int " << stmt_INT_def.IDENT.value.value() << " = " << ;
				/*std::cout << "stmt_INT_def.IDENT.value.value()" << stmt_INT_def.IDENT.value.value() << std::endl;
				std::cout << "stmt_INT_def.IDENT.type" << stmt_INT_def.IDENT.type << std::endl;*/

				std::vector <std::string> vars = gen->m_vars;

				if (!vars.empty() && std::find(vars.begin(), vars.end(), stmt_INT_def.IDENT.value.value()) != vars.end())
				{
					E0202 error(stmt_INT_def.IDENT.position, stmt_INT_def.IDENT.value.value());
					error.Raise();
				}

				//! an INT DEF has:
				//!		- IDENT token which has:
				//!			- a type
				//!			- a value (this is the variable name)
				//!		- NodeExprIntLit expr:
				//!			- 

				gen->m_vars.push_back(stmt_INT_def.IDENT.value.value());
				
				gen->m_output << "int " << stmt_INT_def.IDENT.value.value() << " = ";

				//stmt_INT_def.expr.var.index();
				gen->GenExpr(stmt_INT_def.expr);


				gen->m_output << ";\n";
			}
			void operator()(const NodeStmtIntAssignment& stmt_INT_assignment) const
			{
				std::vector<std::string> vars = gen->m_vars;
				
				// check if the variable hasn't been initialized or the variable list is empty
				if (vars.empty() || std::find(vars.begin(), vars.end(), stmt_INT_assignment.IDENT.value.value()) == vars.end())
				{
					E0202 error(stmt_INT_assignment.IDENT.position, stmt_INT_assignment.IDENT.value.value());
					error.Raise();
				}

				gen->m_output << stmt_INT_assignment.IDENT.value.value() << " =";

				gen->GenExpr(stmt_INT_assignment.expr);

				gen->m_output << ";\n";
			}

			void operator()(const NodeStmtExit& stmt_exit) const
			{
				gen->isExit = true;
			}
		};

		// send in the value of gen as the current instance of this class
		StmtVisitor visitor { .gen = this };

		// visit based on which type of stmt this is
		std::visit(visitor, stmt.var);

		/*struct StmtVisitor {
			Generator* gen;
			void operator()(const NodeStmtExit& stmt_exit) const
			{
				gen->gen_expr(stmt_exit.expr);
				gen->m_output << "    mov rax, 60\n";
				gen->pop("rdi");
				gen->m_output << "    syscall\n";
			}
			void operator()(const NodeStmtLet& stmt_let) const
			{
				if (gen->m_vars.contains(stmt_let.IDENT.value.value())) {
					std::cerr << "Identifier already used: " << stmt_let.IDENT.value.value() << std::endl;
					exit(EXIT_FAILURE);
				}
				gen->m_vars.insert({ stmt_let.IDENT.value.value(), Var {.stack_loc = gen->m_stack_size } });
				gen->gen_expr(stmt_let.expr);
			}
		};

		StmtVisitor visitor{ .gen = this };
		std::visit(visitor, stmt.var);*/
	}

	[[nodiscard]] std::string GenProg()
	{
		// need to add some function to find out what libraries are necessary

		//m_output << "#include <stdlib.h>\n";

		m_output << "int main ()\n";
		m_output << "{\n";

		for (const NodeStmt& stmt : m_prog.stmts)
		{
			GenStmt(stmt);
		}

		// check if there wasn't an exit statement
		if (!isExit)
		{
			m_output << "while (1) {\n\n}\n";
		}

		// so this is up for debate, I could make it so the function always has an exit statement and doesn't hang or I could deal with this in parsing to make sure at least one 
		// exit statement exists, i could also exit with failure or success
		m_output << "return -1;\n";
		m_output << "}";

		return m_output.str();
	}

private:
	const NodeProg m_prog;
	std::stringstream m_output;
	std::vector<std::string> m_vars;
	bool isExit = false;
};