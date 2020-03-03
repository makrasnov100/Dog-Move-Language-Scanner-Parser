#ifndef PARSER_H
#define PARSER_H

#include "SymbolTableEntry.h"
#include <string>
#include <vector>
#include <unordered_map>

enum MsgType {Info,Warning,Error,Success};

class Parser
{
private:
	bool showParseWarnings;
	std::unordered_map<std::string, SymbolTableEntry> symbolTable = {};
	
public:

    Parser(bool showParseWarnings, bool makeCppTranslation);

	std::unordered_map<std::string, SymbolTableEntry> PerformParse(std::vector<Token> &programText);

	//Check for specific term types
	void ProccessProgram(int& idx, std::vector<Token> &tokens);
	void ProccessStmtList(int& idx, std::vector<Token> &tokens);
	void ProccessStmt(int& idx, std::vector<Token> &tokens);
	void ProccessMove(int& idx, std::vector<Token> &tokens);
	void ProccessTurn(int& idx, std::vector<Token>& tokens);
	void ProccessDecl(int& idx, std::vector<Token>& tokens);
	void ProccessAssign(int& idx, std::vector<Token>& tokens);
	void ProccessEnd(int& idx, std::vector<Token>& tokens);
};

#endif
