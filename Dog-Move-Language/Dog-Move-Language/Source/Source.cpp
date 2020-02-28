//[DESCRIPTION]
// Title: Dog-Move-Language Scanner and Parser
// Author: Kostiantyn Makrasnov
// Date: 03/02/20

//[REFERENCES]
// The OOP structure of this assignemnet is adapted/copied from the submitted assignment #3
// Code Location ->  https://github.com/makrasnov100/Adhoc-Scanner-Parser



#include "Scanner.h"
#include "Parser.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>

int main()
{
    //TODO Same as enum in token.h
    std::string tokenStr[] = {"assign", "plus", "minus", "times", "division", "lparen", "rparen", "id", "read", "write", "number", "error", "end"};

    //Setup compiler objects
    Scanner scanner;
    Parser parser(true, true);   //do not show parse warnings, make cpp translation in project directory

    //Setup program to compile
    std::string programText =
        "read A\n"
        "read B\n"
        "sum := (A + B)\n"
        "write sum\n"
        "write sum / 2\n";

    //Begin Compilation
    std::vector<Token> tokens = scanner.PerformScan(programText);

    std::cout << "Program that was parsed: " << std::endl;
    std::cout << programText << std::endl;

    std::cout << "----------------------------------------------- " << std::endl;
    std::cout << "Program tokens from scanner: " << std::endl;
    for(int i = 0; i < tokens.size(); i++)
    {
        std::cout << std::fixed << std::setw(10) << tokenStr[tokens[i].getType()] << " | Value: " << tokens[i].getValue() << std::endl;
    }

    std::cout << "----------------------------------------------- " << std::endl;
    std::cout << "Program trace of matches from parser: " << std::endl;
    parser.PerformParse(tokens);

    //TODO add tracking of symbol table for id declarations

    return 0;
}