//Reference:
// 1) Writing to file with c++: http://www.cplusplus.com/doc/tutorial/files/

#include "Token.h"
#include "Parser.h"
#include <vector>
#include <iostream>
#include <fstream>
#include <set>

Parser::Parser(bool showParseWarnings, bool makeCppTranslation)
{
	level = 0;
	this->showParseWarnings = showParseWarnings;
	this->makeCppTranslation = makeCppTranslation;

	//Creating start and end of cpp program
	if (makeCppTranslation)
	{
		cppPrefix =
			"#include <string>\n"
			"#include <iostream>\n\n"
			"int main()\n{\n";

		cppSuffix = "\treturn 0;\n}";
	}
	else
	{
		cppPrefix = "";
		cppSuffix = "";
	}
}

void Parser::PerformParse(std::vector<Token> &tokens)
{
	if (tokens[tokens.size() - 1].getType() == error)
		std::cout << "[ERROR] Parser WILL fail since scanner found a lexigraphical error [ERROR]" << std::endl;

	int idx = 0; //start index

	//Begin proccesing program recursively
	ProccessProgram(idx, tokens);

	if (tokens[tokens.size() - 1].getType() == error)
		std::cout << "[ERROR] Parser SHOULD HAVE failed since scanner found a lexigraphical error [ERROR]" << std::endl;

	//Export CPP translation if reuired
	if (makeCppTranslation)
		ExportTranslation(tokens);
}

//Root Proccessing (Every function results in idx equaling -1 if not a match) 
void Parser::ProccessProgram(int& idx, std::vector<Token> &tokens)
{
	level++;

	ProccessStmtList(idx, tokens);
	if (idx != -1)
	{
		ProccessEnd(idx, tokens);
		if (idx != -1)
		{
			PreExitMessage(Success, "Program parser has completed work without errors!");
			return;
		}
	}
	
	PreExitMessage(Error, "Program parser has encountered syntax errors!");
	idx = -1;
}

void Parser::ProccessStmtList(int& idx,  std::vector<Token> &tokens)
{
	level++;
	int startIdx = idx;

	ProccessStmt(idx, tokens);
	if (idx != -1)
	{
		ProccessStmtList(idx, tokens);
		if (idx != -1)
		{
			PreExitMessage(Info, "Matched - Stmt_List");
			return;
		}
	}
	else
	{
		PreExitMessage(Info, "Matched - Empty Stmt_List");
		idx = startIdx;
		return;
	}

	PreExitMessage(Warning, "Failed to match Stmt_List");
	idx = -1;
}

void Parser::ProccessStmt(int& idx, std::vector<Token> &tokens)
{
	level++;

	int startIdx = idx;
	std::string startCPP = cppTranslation;
	if (tokens[idx].getType() == id)
	{
		idx++;

		if (tokens[idx].getType() == assign)
		{
			if (!tokens[idx - 1].getDefined())
			{
				cppTranslation += "\tfloat " + tokens[idx - 1].getValue() + " = ";

				for (int i = idx - 1; i < tokens.size(); i++)
					if(tokens[i].getValue() == tokens[idx - 1].getValue())
						tokens[i].setDefined(true);
			}
			else
			{
				cppTranslation += "\t" + tokens[idx - 1].getValue() + " = ";
			}
			idx++;


			ProccessExpr(idx, tokens);
			if (idx != -1)
			{
				cppTranslation += ";\n";
				PreExitMessage(Info, "Matched - Stmt");
				return;
			}
		}
	}
	idx = startIdx;
	cppTranslation = startCPP;


	if (tokens[idx].getType() == read)
	{
		idx++;

		if (tokens[idx].getType() == id)
		{
			AddTranslatedInput(idx, tokens);
			idx++;
			PreExitMessage(Info, "Matched - Stmt");
			return;
		}
	}
	idx = startIdx;


	if (tokens[idx].getType() == write)
	{
		idx++;
		cppTranslation += "\tstd::cout << std::to_string(";

		ProccessExpr(idx, tokens);
		if (idx != -1)
		{
			cppTranslation += ") << std::endl;\n";
			PreExitMessage(Info, "Matched - Stmt");
			return;
		}
	}

	PreExitMessage(Warning, "Failed to match Stmt");
	idx = -1;
	cppTranslation = startCPP;
}

void Parser::ProccessExpr(int& idx, std::vector<Token> &tokens)
{
	level++;

	ProccessTerm(idx, tokens);
	if (idx != -1)
	{
		ProccessTermTail(idx, tokens);
		if (idx != -1)
		{
			PreExitMessage(Info, "Matched - Expr");
			return;
		}
	}

	PreExitMessage(Warning, "Failed to match Expr");
	idx = -1;
}

void Parser::ProccessTermTail(int& idx, std::vector<Token> &tokens)
{
	level++;
	int startIdx = idx;

	ProccessAddOp(idx, tokens);
	if (idx != -1)
	{
		ProccessTerm(idx, tokens);
		if (idx != -1)
		{
			ProccessTermTail(idx, tokens);
			if (idx != -1)
			{
				PreExitMessage(Info, "Matched - TermTail");
				return;
			}
		}
	}
	else
	{
		idx = startIdx;
		PreExitMessage(Info, "Matched - Empty TermTail");
		return;
	}

	PreExitMessage(Warning, "Failed to match TermTail");
	idx = -1;
}

void Parser::ProccessTerm(int& idx, std::vector<Token> &tokens)
{
	level++;

	ProccessFactor(idx, tokens);
	if (idx != -1)
	{
		ProccessFactorTail(idx, tokens);
		if (idx != -1)
		{
			PreExitMessage(Info, "Matched - Term");
			return;
		}
	}

	PreExitMessage(Warning, "Failed to match Term");
	idx = -1;
}

void Parser::ProccessFactorTail(int& idx, std::vector<Token> &tokens)
{
	level++;
	int startIdx = idx;

	ProccessMultOp(idx, tokens);
	if (idx != -1)
	{
		ProccessFactor(idx, tokens);
		if (idx != -1)
		{
			ProccessFactorTail(idx, tokens);
			if (idx != -1)
			{
				PreExitMessage(Info, "Matched - FactorTail");
				return;
			}
		}
	}
	else
	{
		idx = startIdx;
		PreExitMessage(Info, "Matched - Empty FactorTail");
		return;
	}

	PreExitMessage(Warning, "Failed to match FactorTail");
	idx = -1;
}

void Parser::ProccessFactor(int& idx, std::vector<Token> &tokens) 
{
	level++;
	int startIdx = idx;
	std::string startCPP = cppTranslation;
	if (tokens[idx].getType() == lparen)
	{
		cppTranslation += '(';
		idx++;

		ProccessExpr(idx, tokens);
		if (idx != -1)
		{
			if (tokens[idx].getType() == rparen)
			{
				cppTranslation += ')';
				idx++;
				PreExitMessage(Info, "Matched - Factor");
				return;
			}
		}
	}
	idx = startIdx;
	cppTranslation = startCPP;

	if (tokens[idx].getType() == id)
	{
		cppTranslation += tokens[idx].getValue();
		idx++;
		PreExitMessage(Info, "Matched - Factor");
		return;
	}
	idx = startIdx;

	if (tokens[idx].getType() == number)
	{
		cppTranslation += tokens[idx].getValue();
		idx++;
		PreExitMessage(Info, "Matched - Factor");
		return;
	}
	
	PreExitMessage(Warning, "Failed to match Factor");
	idx = -1;
}

void Parser::ProccessAddOp(int& idx, std::vector<Token> &tokens)
{
	level++;

	if (tokens[idx].getType() == plus || tokens[idx].getType() == minus)
	{
		cppTranslation += tokens[idx].getValue();
		idx++;
		PreExitMessage(Info, "Matched - AddOp");
		return;
	}

	PreExitMessage(Warning, "Failed to match AddOp");
	idx = -1;
}

void Parser::ProccessMultOp(int& idx, std::vector<Token> &tokens)
{
	level++;

	if (tokens[idx].getType() == times || tokens[idx].getType() == division)
	{
		cppTranslation += tokens[idx].getValue();
		idx++;
		PreExitMessage(Info, "Matched - MultOp");
		return;
	}

	PreExitMessage(Warning, "Failed to match MultOp");
	idx = -1;
}

void Parser::ProccessEnd(int& idx, std::vector<Token> &tokens)
{
	level++;

	if (tokens[idx].getType() == end)
	{
		idx++;
		PreExitMessage(Info, "Matched - End");
		return;
	}

	PreExitMessage(Warning, "Failed to match End");
	idx = -1;
}

//Translator Functions
void Parser::AddTranslatedInput(int idx, std::vector<Token>& tokens)
{
	if (!tokens[idx].getDefined())
		cppTranslation += "\tfloat " + tokens[idx].getValue() + " = 0;\n";
	cppTranslation += "\tstd::cin >> " + tokens[idx].getValue() + ";\n";
	cppTranslation += "\tstd::cin.clear();\n";
	cppTranslation += "\tstd::cin.ignore(10000, '\\n');\n\n";

	for (int i = idx; i < tokens.size(); i++)
		if (tokens[i].getValue() == tokens[idx].getValue())
			tokens[i].setDefined(true);
}

void Parser::ExportTranslation(std::vector<Token> &tokens)
{
	//Initialize any id's that weren't already to 0s at start of program
	std::string addition = "";
	std::set<std::string> definedIDs;
	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].getType() == id && tokens[i].getDefined())
		{
			definedIDs.insert(tokens[i].getValue());
		}
	}
	for (int i = 0; i < tokens.size(); i++)
	{
		if (tokens[i].getType() == id && definedIDs.find(tokens[i].getValue()) == definedIDs.end())
		{
			addition = "\tfloat " + tokens[i].getValue() + " = 0;\n" + addition;
			definedIDs.insert(tokens[i].getValue());
		}
	}
	if(addition != "")
		cppTranslation = addition + '\n' + cppTranslation;

	//Export To file
	std::ofstream myfile;
	myfile.open("translation.cpp");
	myfile << cppPrefix << cppTranslation << cppSuffix;
	myfile.close();
}


//Console Utility
void Parser::PreExitMessage(MsgType type, std::string msg)
{
	level--;

	if (!showParseWarnings && type == Warning)
		return;

	std::cout << std::string(level*2, ' ') << msg << std::endl;
}