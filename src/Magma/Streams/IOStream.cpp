#include "IOStream.hpp"
#include "..\String.hpp"

#include <cstdio>

bool Magma::STDOutStreamBuffer::Overflow(char32_t character)
{
	// Put UTF-8 character on STDOUT
	char chr[4];
	auto chrSize = UTF8::FromUnicode(character, chr);
	for (size_t i = 0; i < chrSize; ++i)
		if (fputc(chr[i], stdout) == EOF) // Check for EOF
			return false;
	return true;
}

Magma::STDOutStreamBuffer::STDOutStreamBuffer(char32_t * buffer, size_t bufferSize)
	: StreamBuffer(buffer, bufferSize)
{
	
}

bool Magma::STDOutStreamBuffer::Sync()
{
	bool ret = StreamBuffer::Sync();
	if (ret == false)
		return false;
	fflush(stdout);
	return true;
}

Magma::STDInStreamBuffer::STDInStreamBuffer(char32_t * buffer, size_t bufferSize)
	: StreamBuffer(buffer, bufferSize)
{

}

char32_t Magma::STDInStreamBuffer::Underflow()
{
	// Get UTF-8 character from STDIN
	char chr[4];
	int get = '\0';
	
	get = fgetc(stdin);
	if (get == EOF) // Check for EOF
		return StreamBuffer::EndOfFile;
	chr[0] = get;

	auto chrSize = UTF8::GetCharSize(chr);
	for (size_t i = 1; i < chrSize; ++i)
	{
		get = fgetc(stdin);
		if (get == EOF) // Check for EOF
			return StreamBuffer::EndOfFile;
		chr[i] = get;
	}

	return UTF8::ToUnicode(chr);
}
