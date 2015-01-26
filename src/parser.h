#ifndef COMPONENT_PARSER_H
#define COMPONENT_PARSER_H

// error codes.
#define WARNING 31
#define ERROR 32
#define FATAL 33

class Error
{
	String _code, _flag;
	bufferIndex _line;
	int _severity;

	public:
	Error(String = "", String = "", bufferIndex = -1, int = ERROR);
	Error(const Error&);
	Error& operator= (const Error&);

	String code() const;
	String flag() const;
	String message() const;
	bufferIndex lineNumber() const;
	int severity() const;

	bool setLineNumber(bufferIndex);
};

idList errorCodes, errorDesc;
bool importErrorCodes();

typedef Queue<Token> RPN;

#include "variables.h"

// parser status.
#define SUCCESS 41
#define PENDING 42
#define STARTED 43
#define FAILED 44

class Parser
{
	private:
	VariableScope variables; // stores globals.
	Vector <Function> functions;
	Vector <Error> errors;
	RPN output;
	int status;
	
	// lexer interaction interface:
	Lexer *lexer;

	public:
	Parser(Lexer*, String);
	~Parser();

	// interface
	bool sendError(Error);
	Function getFunction(String);

	// parsing procedures:
	bool parseSource();

	RPN parseBlock(bufferIndex = 0);
	RPN parseStatement(Infix);
	RPN parseDeclaration(Infix, tokenType);

	/* static parsing procedures: */
	// assumes that the tokens have already been read.
	static RPN expressionToRPN(Infix, Vector<Error>&, VariableScope&, Function (*)(String));
	// checks whether the RPN is valid.
	static bool validateRPN(RPN);

};

#endif /* COMPONENT_PARSER_H */