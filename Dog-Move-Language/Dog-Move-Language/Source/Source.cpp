//[DESCRIPTION]
// Title: Dog-Move-Language Scanner and Parser
// Author: Kostiantyn Makrasnov
// Date: 03/02/20

//[REFERENCES]
// The OOP structure of this assignemnet is adapted/copied from the submitted assignment #3
// Code Location ->  https://github.com/makrasnov100/Adhoc-Scanner-Parser
// Substring parameters -> http://www.cplusplus.com/reference/string/string/substr/
// String constructor for char conversion -> https://stackoverflow.com/questions/17201590/c-convert-from-1-char-to-string
// Unordered map methods -> http://www.cplusplus.com/reference/unordered_map/unordered_map/find/


#include "Scanner.h"
#include "Parser.h"

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <unordered_map>

int main()
{
    //TODO Same as enum in token.h
    std::string tokenStr[] = { "lparen", "rparen", "take", "drop", "move", "turn", "var", "id", "assignOp", "count", "dir", "error", "end"};

    //Setup compiler objects
    Scanner scanner;
    Parser parser(true, true);   //do not show parse warnings, make cpp translation in project directory

    //Setup program to compile
    std::string programText =
        "var $rightgo\n"
        "$rightgo := 2\n"
        "move(2)\n"
        "take\n"
        "turn(l)\n"
        "move(4)\n"
        "turn(r)\n"
        "move(2)\n"
        "var $coke\n"
        "var $steak\n"
        "var $meat\n"
        "drop";

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
    std::cout << "Resulting of program parsing: " << std::endl;
    std::unordered_map<std::string, SymbolTableEntry> symbolTable = parser.PerformParse(tokens);

    std::cout << "----------------------------------------------- " << std::endl;
    std::cout << "Final symbol table: " << std::endl;
    std::cout << "identifier: index " << std::endl;
    for (auto& x : symbolTable)
        std::cout << x.first << ": " << std::to_string(x.second.index) << std::endl;

    return 0;
}