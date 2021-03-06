#ifndef SRC_LEXER_H_
#define SRC_LEXER_H_

// Token types:
#define DIRECTIVE -1
#define UNKNOWN 0
#define KEYWORD 1
  #define CONSTANT 11
#define OPERATOR 2
  #define UNARYOP  21
  #define BINARYOP 22
  #define MULTINARYOP 23
  #define POST 24
  #define PRE 25
#define PUNCTUATOR 3
#define IDENTIFIER 4
  #define VARIABLE 41
  #define FUNCTION 42
  #define CLASS    43
  #define ARRAY    44
  #define OBJECT   45
#define LITERAL 5
  #define STRING 51
  #define NUMBER 52
  #define BOOLEAN 53
  #define INTEGER 54
#define HASHED 6

#define TABLEN 4
#define MAX_ID_LENGTH 18

typedef int tokenType;
typedef __SIZETYPE bufferIndex;
typedef Vector<String> idList;

// error codes.
#define ERROR_WARNING 31
#define ERROR_ERROR 32
#define ERROR_FATAL 33
#define ERROR_RUNTIME 34

/*******************************
* Class Error
* Stores information about an error, along with a flag.
* member message() returns a user friendly format of the error as a string.
********************************/
class Error {
  String _code, _flag;
  bufferIndex _line;
  int _severity;

 public:
  explicit Error(String cd = "", String fg = "", bufferIndex ln = -1, int s = ERROR_ERROR);
  Error(const Error &e);
  Error& operator= (const Error &e);

  String code() const;
  String flag() const;
  String message() const;
  bufferIndex lineNumber() const;
  int severity() const;

  void setLineNumber(bufferIndex ln);
};

idList errorCodes, errorDesc;

// global to store all reserved words, and symbols.
idList Keywords,
       Constants,  // stores all predefined literal constant keywords
       InbuiltFunctionList,  // stores all inbuilt functions.
       Punctuators,  // all symbols which separate lexer input.
       Opstarts,
       unaryOperators,
       binaryOperators;

/*******************************
* Class Token:
*   Stores information about a single keyword/id/literal/operator
*
*******************************/
class Token {
 private:
  tokenType _type, _subtype;
  bufferIndex _line, _indent;
  String _value;
 public:
  // constructors
  Token(const Token &t);
  explicit Token(String val = "", tokenType t = UNKNOWN, tokenType st = UNKNOWN, bufferIndex ln = -1, bufferIndex in = 0);
  Token& operator= (const Token &t);

  // properties
  tokenType type() const;
  tokenType subtype() const;
  bufferIndex lineNumber() const;
  bufferIndex indent() const;
  String value() const;
  // mutators
  void setType(tokenType t);
  void setSubtype(tokenType st);
  void setLineNumber(bufferIndex ln);
  void setIndent(bufferIndex in);
  void setValue(String s);
};

// Directive Tokens
Token eofToken, nullToken, newlineToken;
// Literal tokens
Token trueToken, falseToken, nullvalToken;
// module to install the reserved words, and other lexer data
bool loadLexerData();
bool importErrorCodes(ifstream& ecreader);

typedef Queue<Token> Infix;

/******************************
* The Lexer Class:
* Takes source code from a file, and converts them into tokens.
* member getToken(): extracts and returns the next token.
*******************************/
class Lexer {
 private:
  bufferIndex line, indent;
  ifstream source;  // handles the source code file.
  Vector<Error> errors;  // stores all lexer errors
  Deque<Token> innerBuffer;  // stores tokens that have been sent back.

 private:
  int trim();
  int endLine();
  String readString();
  String readNumber();
  String readIdentifier();
  String readOperator();

 public :
  explicit Lexer(String data);
  ~Lexer();

  Vector<Error> getErrors() const;
  Token getToken();
  bool putbackToken(Token tok);
  Infix getTokensTill(String delim);
  bool ended();

  // static members:
  static Token toToken(String val);
  static bool isValidIdentifier(String val);
  static String entityMap(String val);
  static String tokenToString(Token t);
  static String stringToLiteral(String val);
  static String typeToString(tokenType ty);
};

#endif  // SRC_LEXER_H_
