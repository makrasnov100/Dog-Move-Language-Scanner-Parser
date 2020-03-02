#ifndef SCANNER_H
#define SCANNER_H

#include "Token.h"
#include <string>
#include <vector>



class Scanner
{
public:
    Scanner();

    std::vector<Token> PerformScan(std::string programText);
	Token checkKeyword(std::string keyword, TokenType type, std::string& program, int& counter);

};

#endif