#include <iostream>

#include <Magma\String.hpp>

using namespace Magma;

int main(int argc, char** argv)
{
	try
	{
		String str = u8"Test!";

		String str2 = str + u8" This was appended! ";
		str2 += u8"This was going to be a new line, but the new line was popped.";

		str2.Clear();
		str2.Insert(0, u8"A B");

		std::cout << str2.CString() << std::endl;

		auto split = str2.Split(U' ');
		for (auto& substring : split)
			std::cout << "'" << substring.CString() << "' ";
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cin.get();
}
