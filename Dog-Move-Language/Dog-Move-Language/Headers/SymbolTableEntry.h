#ifndef SYMBOL_TABLE_ENTRY_H
#define SYMBOL_TABLE_ENTRY_H

#include <string>

class SymbolTableEntry
{

public:
	int index;
	std::string symbol;

	SymbolTableEntry(int index, std::string symbol)
	{
		this->index = index;
		this->symbol = symbol;
	}
};

#endif