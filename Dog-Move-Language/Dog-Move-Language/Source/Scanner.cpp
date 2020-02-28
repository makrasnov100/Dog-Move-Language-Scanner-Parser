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
            case '(':
                tokens.push_back(Token(text.substr(counter, 1), lparen));
                break;
            case '\r':
            case '\n':
            case ' ':
            case '\t':
                break;
            default:
                //check for id
        }
        counter++;
    }

    //write an end of program token for parser to use
    tokens.push_back(Token("", end));

    return tokens;
}


//Skips to end of comment by searching for a remination of a started comment
//NOTE: starts on the charecter that openend the comment
int Scanner::ScanUntilEndOfComment(int counter, std::string &text)
{
    
}

