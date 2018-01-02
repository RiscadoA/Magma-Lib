#include <iostream>
#include <array>

#include <Magma\String.hpp>
#include <Magma\Streams\IOStream.hpp>

using namespace Magma;

int main(int argc, char** argv)
{
	//freopen("test.txt", "r", stdin);

	try
	{
		std::array<char32_t, 256> stdOutBuffer;
		std::array<char32_t, 256> stdInBuffer;

		STDOutStreamBuffer stdOut(stdOutBuffer.data(), stdOutBuffer.size());
		STDInStreamBuffer stdIn(stdInBuffer.data(), stdInBuffer.size());

		auto chr = stdIn.GetChar();

		std::cout << std::endl << std::hex << chr << std::endl;

		stdOut.PutChar(chr);

		stdOut.Sync();
	}
	catch (std::exception& e)
	{
		std::cout << e.what() << std::endl;
	}

	for (size_t i = 0; i < 2; ++i)
		std::cin.get();
}
