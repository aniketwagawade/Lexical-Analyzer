#ifndef LEXER_H
#define LEXER_H

// Maximum number of keywords supported by the lexer
#define MAX_KEYWORDS 20

// Maximum size allowed for a single token lexeme
#define MAX_TOKEN_SIZE 100

// Token types recognized by the lexical analyzer
typedef enum {
    KEYWORD,
    OPERATOR,
    SPECIAL_CHARACTER,
    CONSTANT,
    IDENTIFIER,
    END_OF_FILE,
    UNKNOWN
} TokenType;

// Token structure holding lexeme and its type
typedef struct 
{
    char lexeme[MAX_TOKEN_SIZE];
    TokenType type;
} Token;

// Function declarations for lexer operation
void initializeLexer(const char* filename);
Token getNextToken();
void categorizeToken(Token* token);
int isStringLiteral(const char *str);
int isKeyword(const char* str);
int isOperator(const char* str);
int isSpecialCharacter(char ch);
int isConstant(const char* str);
int isIdentifier(const char* str);

#endif
