

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
				if ((counter + 1 < text.size()) && (text[counter + 1] == '='))
				{
					tokens.push_back(Token(std::string(":="), assignOp));
					counter++;
				}
				else
				{
					tokens.push_back(Token(std::string(""), error));
					counter = -1;
				}
				break;
			case 't':
				if (counter + 1 < text.size())
				{
					if (text[counter + 1] == 'a') // start of take
					{
						tokens.push_back(checkKeyword("take", take, text, counter));
					}
					else if (text[counter + 1] == 'u')	// start of turn
					{
						tokens.push_back(checkKeyword("turn", turn, text, counter));
					}
					else
					{
						tokens.push_back(Token(std::string(""), error));
						counter = -1;
					}
				}
				break;
			case 'd':
				tokens.push_back(checkKeyword("drop", drop, text, counter));
				break;
			case 'm':
				tokens.push_back(checkKeyword("move", move, text, counter));
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
			{
				int counterSave = counter;
				counter++;
				while (counter < text.size() && std::isalpha(text[counter]) && std::islower(text[counter]))
				{
					counter++;
				}
				counter--;

				if (counterSave == counter)	// check if no identifier
					counter = -1;

				if (counter > 0 && counterSave < counter)
					tokens.push_back(Token(text.substr(counterSave, counter - counterSave + 1), id));
				else
				{
					tokens.push_back(Token(std::string("$null"), error));
					counter = -1;
				}
				break;
			}
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
				tokens.push_back(Token(std::string(1, text[counter]), count));
				break;
			case 'l':
			case 'r':
				tokens.push_back(Token(std::string(1, text[counter]), dir));
				break;
            default:
				tokens.push_back(Token("unused symbol - " + std::string(1, text[counter]), error));
				counter = -1;
        }

		//For all error cases return with lexigrathical error
		//TODO: check if error token is actually puwshed to token vector
		if (counter == -1)
			return tokens;

        counter++;
    }

    //Write an end of program token for parser to use
    tokens.push_back(Token(std::string(""), end));

    return tokens;
}

//Checks if at current counter location the keyword is present
//keyword: the word presence of whch needs to be checked
//program: the program code that the key word is supposebly in
//counter: the char index in code at which the scanner is currently at
Token Scanner::checkKeyword(std::string keyword, TokenType type, std::string &program, int &counter)
{
	if (counter + (keyword.size()-1) < program.size())
	{
		if (program.substr(counter, keyword.size()) == keyword)
		{
			Token tempToken = Token(program.substr(counter, keyword.size()), type);
			counter += keyword.size()-1;
			return tempToken;
		}
		else
		{
			Token tempToken = Token(program.substr(counter, keyword.size()), error);
			counter = -1;
			return tempToken;
		}
	}
	else
	{
		Token tempToken = Token(program.substr(counter, program.size() - counter), error);
		counter = -1;
		return tempToken;
	}
}

