#include <string>
#include <sstream>
#include <iostream>
#include <unordered_map>

#include "parser.h"

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
				gen->m_output << "return " << expr_int_literal.int_lit.value.value() << ";\n";
			}
			void operator()(const NodeExprIdent& expr_ident) const
			{
				// define integer 
				gen->m_output << "int " << expr_ident.ident.value.value() << "=";
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
				// access GenExpr from parent class
				gen->GenExpr(stmt_return.expr);
			}
			void operator()(const NodeStmtIntDef& stmt_INT_def) const
			{
				gen->GenExpr(stmt_INT_def.expr);
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

		m_output << "#include <stdlib.h>\n";

		m_output << "int main ()\n";
		m_output << "{\n";

		for (const NodeStmt& stmt : m_prog.stmts)
		{
			GenStmt(stmt);
		}
		// so this is up for debate, I could make it so the function always has an exit statement and doesn't hang or I could deal with this in parsing to make sure at least one 
		// exit statement exists, i could also exit with failure or success
		m_output << "exit(-1);\n";
		m_output << "}";

		return m_output.str();
	}

private:
	/*void push(const std::string& reg)
	{
		m_output << "    push " << reg << "\n";
		m_stack_size++;
	}

	void pop(const std::string& reg)
	{
		m_output << "    pop " << reg << "\n";
		m_stack_size--;
	}

	struct Var {
		size_t stack_loc;
	};*/

	const NodeProg m_prog;
	std::stringstream m_output;
	//size_t m_stack_size = 0;
	//std::unordered_map<std::string, Var> m_vars {};
};