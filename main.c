#include <stdio.h>
#include "lexer.h"

int main(int argc, char *argv[]) 
{
    // Check if input file is provided
    if (argc < 2)
    {
        printf("Usage : %s <.c file>\n", argv[0]);
        return 1;
    }

    // Display file opening and parsing status
    printf("Open      : %s : Success\n", argv[1]);
    printf("Parsing   : %s : Started\n\n", argv[1]);

     // Initialize lexer by opening the source file
    initializeLexer(argv[1]);

    Token token;

    // Fetch tokens until END_OF_FILE is reached
    while (1)
    {
        token = getNextToken();

        if (token.type == END_OF_FILE)
            break;

            
        // Print token type along with lexeme
        switch (token.type)
        {
            case KEYWORD:
                printf("Keyword   : %s\n", token.lexeme);
                break;

            case IDENTIFIER:
                printf("Identifier: %s\n", token.lexeme);
                break;

            case CONSTANT:
                printf("Literal   : %s\n", token.lexeme);
                break;

            case OPERATOR:
                printf("Operator  : %s\n", token.lexeme);
                break;

            case SPECIAL_CHARACTER:
                printf("Operator  : %s\n", token.lexeme);
                break;

            default:
                printf("Unknown   : %s\n", token.lexeme);
                break;
        }
    }

    // Indicate finishing of parsing
    printf("\nParsing   : %s : Done\n", argv[1]);

    return 0;
}