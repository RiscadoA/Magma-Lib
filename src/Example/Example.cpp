#include <iostream>

#include <Magma\String.hpp>

using namespace Magma;

int main(int argc, char** argv)
{
	try
	{
		String str = u8"Test!";

		String str2 = str + u8" This was appended! \n";
		str2.Pop();
		str2 += u8"This was going to be a new line, but the new line was popped.";

		std::cout << str2.CString();
	}
	catch (InvalidStringError& e)
	{
		std::cout << e.what() << std::endl;
	}
	catch(UTF8::InvalidCharacterError& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cin.get();
}
