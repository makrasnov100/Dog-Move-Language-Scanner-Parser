#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>

enum MsgType {Info,Warning,Error,Success};

class Parser
{
private:
	int level;
	bool showParseWarnings;
	bool makeCppTranslation;

	std::string cppTranslation;
	std::string cppPrefix;
	std::string cppSuffix;
public:

    Parser(bool showParseWarnings, bool makeCppTranslation);

	void PerformParse(std::vector<Token> &programText);

	//Check for specific term types
	void Parser::ProccessProgram(int& idx, std::vector<Token> &tokens);
	void Parser::ProccessStmtList(int& idx, std::vector<Token> &tokens);
	void Parser::ProccessStmt(int& idx, std::vector<Token> &tokens);
	void Parser::ProccessExpr(int& idx, std::vector<Token> &tokens);
	void Parser::ProccessTermTail(int& idx, std::vector<Token> &tokens);
	void Parser::ProccessTerm(int& idx, std::vector<Token> &tokens);
	void Parser::ProccessFactorTail(int& idx, std::vector<Token> &tokens);
	void Parser::ProccessFactor(int& idx, std::vector<Token> &tokens);
	void Parser::ProccessAddOp(int& idx, std::vector<Token> &tokens);
	void Parser::ProccessMultOp(int& idx, std::vector<Token> &tokens);
	void Parser::ProccessEnd(int& idx, std::vector<Token> &tokens);

	//Translation functions
	void AddTranslatedInput(int idx, std::vector<Token> &identifierTarget);
	void ExportTranslation(std::vector<Token>& tokens);

	//Console Utilities
	void Parser::PreExitMessage(MsgType type, std::string msg);
};

#endif
