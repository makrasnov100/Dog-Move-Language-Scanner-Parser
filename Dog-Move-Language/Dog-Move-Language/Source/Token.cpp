#include "Token.h"

//Constructor
Token::Token(std::string value, TokenType type)
{
	this->type = type;
	this->value = value;

	//translation specific
	defined = false;
}

//Accessors / Mutators
std::string Token::getValue() { return value; }
bool Token::getDefined() { return defined; }
TokenType Token::getType() { return type; }

void  Token::setDefined(bool isDefined) { defined = isDefined; }
