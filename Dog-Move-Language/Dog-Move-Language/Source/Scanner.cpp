//References:
//substring parameters: http://www.cplusplus.com/reference/string/string/substr/

#include "Scanner.h"
#include "Token.h"
#include <string>
#include <vector>
#include <cctype>   //isdigit
#include <iostream>

Scanner::Scanner()
{
    //Setup Scanner
}

std::vector<Token> Scanner::PerformScan(std::string text)
{
    //Variables keeping track of the scanning progress
    std::vector<Token> tokens;
    int counter = 0;

    //Skip initial white space
    while(text[counter] == ' ' || text[counter] == '\r' || text[counter] == '\n' || text[counter] == '\t')
    {
        counter++;
    }

    //Run scanner until lexigraphical error occurs or reached end of program
    while(counter < text.size() && counter != -1)
    {
        //Check one charecter a time branching into peeks and loops for more complex tokens
		switch (text[counter])
        {
			case ':':
				if (counter + 1 < text.length && text[counter+1] == '=')
					tokens.push_back(Token(":=", assignOp));
				else
					tokens.push_back(Token("", error));
			case 't':
				if (counter + 1 < text.length)
				{
					if (text[counter+1] == 'a') // start of take
					{
						tokens.push_back(checkKeyword("take", take, text, counter));
					}
					else if (text[counter+1] == 'u')	// start of turn
					{
						tokens.push_back(checkKeyword("turn", turn, text, counter));
					}
					else
					{
						tokens.push_back(Token("", error));
						counter == -1;
					}
				}
				break;
			case 'd':
				tokens.push_back(checkKeyword("drop", take, text, counter));
				break;
			case 'm':
				tokens.push_back(checkKeyword("move", take, text, counter));
				break;
			case 'v':
				tokens.push_back(checkKeyword("var", var, text, counter));
				break;
            case '(':
                tokens.push_back(Token(text.substr(counter, 1), lparen));
                break;
			case ')':
				tokens.push_back(Token(text.substr(counter, 1), rparen));
				break;
            case '\r':
            case '\n':
            case ' ':
            case '\t':
                break;
			case '$':
				//check for id
            default:
                //nothing else allowed print error if present

        }

		//For all error cases return with lexigrathical error
		//TODO: check if error token is actually puwshed to token vector
		if (counter == -1)
			return;

        counter++;
    }

    //Write an end of program token for parser to use
    tokens.push_back(Token("", end));

    return tokens;
}

//Checks if at current counter location the keyword is present
//keyword: the word presence of whch needs to be checked
//program: the program code that the key word is supposebly in
//counter: the char index in code at which the scanner is currently at
Token Scanner::checkKeyword(std::string keyword, TokenType type, std::string &program, int &counter)
{
	if (counter + (keyword.length-1) < program.length)
	{
		if (program.substr(counter, keyword.length) == keyword)
		{
			return Token(program.substr(counter, keyword.length), type);
			counter += keyword.length;
		}
		else
		{
			return Token(program.substr(counter, keyword.length), error);
			counter = -1;
		}
	}
	else
	{
		return Token(program.substr(counter, program.length - counter), error);
		counter = -1;
	}
}

