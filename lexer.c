#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"

/*---------Keyword used for lexer----------*/
static const char* keywords[MAX_KEYWORDS] = 
{
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"
};

// Basic operator and special symbol sets
static const char* operators = "+-*/%=!<>|&";
static const char* specialCharacters = ",;{}()[]";

// File pointer used throughout lexer
static FILE* sourceFile = NULL;

// Initialize lexer by opening input file for tokenization
void initializeLexer(const char* filename)
{
    sourceFile = fopen(filename, "r");
    if (!sourceFile)
    {
        perror("Error opening file");
    }
}

// Main function to extract next token from the source code
Token getNextToken()
{
    Token token;
    int ch, i = 0;

    // If file not opened, return END_OF_FILE immediately
    if (!sourceFile)
    {
        strcpy(token.lexeme, "");
        token.type = END_OF_FILE;
        return token;
    }

    // Skip all whitespace characters 
    while ((ch = fgetc(sourceFile)) != EOF && isspace(ch));

    // If EOF after skipping spaces
    if (ch == EOF)
    {
        strcpy(token.lexeme, "");
        token.type = END_OF_FILE;
        return token;
    }

    // Identifier or keyword
    if (isalpha(ch) || ch == '_')
    {
        token.lexeme[i++] = ch;
        while ((ch = fgetc(sourceFile)) != EOF && (isalnum(ch) || ch == '_'))
            token.lexeme[i++] = ch;
        ungetc(ch, sourceFile);
    }
    // Numeric constant
    else if (isdigit(ch))
    {
        token.lexeme[i++] = ch;
        while ((ch = fgetc(sourceFile)) != EOF && isdigit(ch))
            token.lexeme[i++] = ch;
        ungetc(ch, sourceFile);
    }
    // Updated STRING LITERAL handling — \n → 0
    else if (ch == '\"')
    {
        token.lexeme[i++] = ch;   // opening "

        while ((ch = fgetc(sourceFile)) != EOF && ch != '\"')
        {
            if (ch == '\\')        // escape sequence found
            {
                int next = fgetc(sourceFile);

                if (next == 'n')   //  convert \n to 0
                {
                    token.lexeme[i++] = '0';
                }
                else               // if not \n, keep backslash and push back next
                {
                    token.lexeme[i++] = '\\';
                    ungetc(next, sourceFile);
                }
            }
            else
            {
                token.lexeme[i++] = ch;
            }
        }

        token.lexeme[i++] = '\"';  // closing 
    }
    // Operators
    else if (strchr(operators, ch))
    {
        token.lexeme[i++] = ch;
        int next = fgetc(sourceFile);

        if (strchr(operators, next) &&
            (next == '=' || next == ch))
        {
            token.lexeme[i++] = next;
        }
        else
        {
            ungetc(next, sourceFile);
        }
    }

    // Special character
    else if (strchr(specialCharacters, ch))
    {
        token.lexeme[i++] = ch;
    }
    else
    {
        token.lexeme[i++] = ch;
    }

    token.lexeme[i] = '\0';
    categorizeToken(&token);

    return token;
}

// Categorize given token and assign correct TokenType
void categorizeToken(Token* token)
{
    if (isKeyword(token->lexeme))
        token->type = KEYWORD;

    else if (isOperator(token->lexeme))
        token->type = OPERATOR;

    else if (isSpecialCharacter(token->lexeme[0]) && strlen(token->lexeme) == 1)
        token->type = SPECIAL_CHARACTER;

    else if (isStringLiteral(token->lexeme))   
        token->type = CONSTANT;

    else if (isConstant(token->lexeme))
        token->type = CONSTANT;

    else if (isIdentifier(token->lexeme))
        token->type = IDENTIFIER;

    else
        token->type = UNKNOWN;
}

// check if lexeme is a valid string literal ("...")
int isStringLiteral(const char *str)
{
    int len = strlen(str);
    return (len >= 2 && str[0] == '\"' && str[len - 1] == '\"');
}

// check if lexeme matches any C keyword
int isKeyword(const char* str)
{
    for (int i = 0; i < MAX_KEYWORDS; i++)
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    return 0;
}

// Check if lexeme is an operator (single or double)
int isOperator(const char* str)
{
    if (strlen(str) == 1)
        return strchr(operators, str[0]) != NULL;

    return (
        strcmp(str, "==") == 0 ||
        strcmp(str, "<=") == 0 ||
        strcmp(str, ">=") == 0 ||
        strcmp(str, "!=") == 0 ||
        strcmp(str, "&&") == 0 ||
        strcmp(str, "||") == 0
    );
}

// Check if given character is a special symbol
int isSpecialCharacter(char ch)
{
    return strchr(specialCharacters, ch) != NULL;
}

// Check if lexeme is a numeric constant (all digits)
int isConstant(const char* str)
{
    for (int i = 0; str[i]; i++)
        if (!isdigit(str[i]))
            return 0;
    return 1;
}

// Check if lexeme is a valid C identifier
int isIdentifier(const char* str)
{
    if (!isalpha(str[0]) && str[0] != '_')
        return 0;

    for (int i = 1; str[i]; i++)
        if (!isalnum(str[i]) && str[i] != '_')
            return 0;

    return 1;
}
