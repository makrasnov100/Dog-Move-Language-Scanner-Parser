#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType { assign, plus, minus, times, division, lparen, rparen, id, read, write, number, error, end};

class Token
{
private:
	std::string value;
	TokenType type;
	bool defined;

public:
	Token( std::string value, TokenType type);
	
	std::string getValue();
	bool getDefined();
	TokenType getType();

	void setDefined(bool isDefined);
};

#endif