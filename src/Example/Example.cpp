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

		auto split = str2.Split(u8"This");
		for (auto& substring : split)
			std::cout << "'" << substring.CString() << "' ";
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	std::cin.get();
}
