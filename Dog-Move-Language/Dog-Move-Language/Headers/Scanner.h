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
    int ScanUntilEndOfComment(int counter, std::string &text);

};

#endif