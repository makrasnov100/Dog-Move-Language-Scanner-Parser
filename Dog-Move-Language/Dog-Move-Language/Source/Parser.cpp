//Reference:
// 1) Writing to file with c++: http://www.cplusplus.com/doc/tutorial/files/

#include "Token.h"
#include "Parser.h"
#include "SymbolTableEntry.h"
#include <vector>
#include <iostream>
#include <fstream>

Parser::Parser(bool showParseWarnings, bool makeCppTranslation)
{
	this->showParseWarnings = showParseWarnings;
}

std::unordered_map<std::string, SymbolTableEntry> Parser::PerformParse(std::vector<Token> &tokens)
{
	int idx = 0; //start index

	//Begin proccesing program recursively
	ProccessProgram(idx, tokens);
	if (idx == -1)
		std::cout << "Program parser determined that the program is INVALID!" << std::endl;
	else
		std::cout << "Program parser determined that the program is VALID!" << std::endl;

	if (tokens[tokens.size() - 1].getType() == error)
		std::cout << "Parser failed since scanner found a lexigraphical error!" << std::endl;

	return symbolTable;
}

//Root Proccessing (Every function results in idx equaling -1 if not a match) 
void Parser::ProccessProgram(int& idx, std::vector<Token> &tokens)
{
	ProccessStmtList(idx, tokens);
	if (idx != -1)
	{
		ProccessEnd(idx, tokens);
		if (idx != -1)
		{
			return;
		}
	}
	
	idx = -1;
}

void Parser::ProccessStmtList(int& idx,  std::vector<Token> &tokens)
{
	int startIdx = idx;

	ProccessStmt(idx, tokens);
	if (idx != -1)
	{
		ProccessStmtList(idx, tokens);
		if (idx != -1)
		{
			return;
		}
	}
	else
	{
		idx = startIdx;
		return;
	}

	idx = -1;
}

void Parser::ProccessStmt(int& idx, std::vector<Token> &tokens)
{
	if (tokens[idx].getType() == take || tokens[idx].getType() == drop)
	{
		idx++;
		return;
	}

	int startIdx = idx;
	ProccessMove(idx, tokens);
	if (idx == -1)
		idx = startIdx;	//failed matching move
	else
		return;

	ProccessTurn(idx, tokens);
	if (idx == -1)
		idx = startIdx; //failed matching turn
	else
		return;

	ProccessDecl(idx, tokens);
	if (idx == -1)
		idx = startIdx; //failed matching declaration
	else
		return;

	ProccessAssign(idx, tokens);
	if (idx != -1)
		return;
	
	idx = -1;	//failed matching all statment types
}

void Parser::ProccessMove(int& idx, std::vector<Token>& tokens)
{
	if (idx + 3 < tokens.size()) //checking length
	{
		if (tokens[idx].getType() == move &&		//checking terminal tokens LL(3)
			tokens[idx + 1].getType() == lparen &&
			tokens[idx + 2].getType() == count &&
			tokens[idx + 3].getType() == rparen)
		{
			idx += 4;
			return;
		}
	}

	idx = -1;	//failed matching
}

void Parser::ProccessTurn(int& idx, std::vector<Token>& tokens)
{
	if (idx + 3 < tokens.size()) //checking length
	{
		if (tokens[idx].getType() == turn &&		//checking terminal tokens LL(3)
			tokens[idx + 1].getType() == lparen &&
			tokens[idx + 2].getType() == dir &&
			tokens[idx + 3].getType() == rparen)
		{
			idx += 4;
			return;
		}
	}

	idx = -1;	//failed matching
}

void Parser::ProccessDecl(int& idx, std::vector<Token>& tokens)
{
	if (idx + 1 < tokens.size()) //checking length
	{
		if (tokens[idx].getType() == var &&		//checking terminal tokens
			tokens[idx + 1].getType() == id)
		{
			//Check ifidentifier is already being used
			std::unordered_map<std::string, SymbolTableEntry>::const_iterator searchResult = symbolTable.find(tokens[idx + 1].getValue());

			if (searchResult == symbolTable.end())	//identifier not declared anywhere yet
			{
				//Add identifier to symbol table
				symbolTable.insert(std::pair<std::string, SymbolTableEntry> (tokens[idx + 1].getValue(),
																			 SymbolTableEntry(idx + 1,
																							  tokens[idx + 1].getValue())));
			}
			else
			{
				std::cout << "Redeclaration of variable - " << tokens[idx + 1].getValue() << " - parser error!" << std::endl;
				idx = -1;
				return;
			}

			idx += 2;
			return;
		}
	}

	idx = -1;	//failed matching
}

void Parser::ProccessAssign(int& idx, std::vector<Token>& tokens)
{
	if (idx + 2 < tokens.size()) //checking length
	{
		if (tokens[idx].getType() == id &&		//checking terminal tokens
			tokens[idx + 1].getType() == assignOp &&
			tokens[idx + 2].getType() == count)
		{
			idx += 3;
			return;
		}
	}

	idx = -1;	//failed matching
}

void Parser::ProccessEnd(int& idx, std::vector<Token> &tokens)
{

	if (tokens[idx].getType() == end)
	{
		idx++;
		return;
	}

	idx = -1;
}