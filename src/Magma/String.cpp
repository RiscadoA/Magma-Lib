#include "String.hpp"

#include <cstring>

bool Magma::UTF8::IsValid(const char * utf8String)
{
	const char* ptr = utf8String;

	try
	{
		while (*ptr != '\0')
		{
			ptr += GetCharSize(ptr);
			UTF8::ToUnicode(ptr);
		}
	}
	catch (InvalidCharacterError chr)
	{
		return false;
	}

	return true;
}

size_t Magma::UTF8::GetCharSize(char32_t character)
{
	if (character <= 0x00007F)
		return 1;
	if (character <= 0x0007FF)
		return 2;
	if (character <= 0x00FFFF)
		return 3;
	if (character <= 0x10FFFF)
		return 4;
	throw InvalidCharacterError("Couldn't get UTF-8 size from character, invalid unicode character (unicode point > 0x10FFFF)");
}

size_t Magma::UTF8::GetCharSize(const char * utf8Char)
{
	if ((*utf8Char & 0b1000'0000) == 0b0000'0000)
		return 1;
	if ((*utf8Char & 0b1110'0000) == 0b1100'0000)
		return 2;
	if ((*utf8Char & 0b1111'0000) == 0b1110'0000)
		return 3;
	if ((*utf8Char & 0b1111'1000) == 0b1111'0000)
		return 4;

	throw InvalidCharacterError("Couldn't get UTF-8 size from character, invalid UTF-8 character (invalid prefix)");
}

size_t Magma::UTF8::GetStringLength(const char * utf8String)
{
	size_t length = 0;
	const char* ptr = utf8String;

	while (*ptr != '\0')
	{
		auto chrSize = GetCharSize(ptr);
		length += chrSize;
		ptr += chrSize;
	}

	return length;
}

char32_t Magma::UTF8::ToUnicode(const char * utf8Char)
{
	char32_t chr = 0;
	auto size = GetCharSize(utf8Char);
	switch (size)
	{
		case 1:
			chr |= utf8Char[0];
			break;
		case 2:
			chr |= (utf8Char[0] & 0b0001'1111) << 6;
			if ((utf8Char[1] & 0b1100'0000) != 0b1000'0000)
				throw InvalidCharacterError("Couldn't convert UTF-8 character to unicode character, invalid UTF-8 character (second byte has wrong/no prefix)");
			chr |= (utf8Char[1] & 0b0011'1111) << 0;
			break;
		case 3:
			chr |= (utf8Char[0] & 0b0000'1111) << 12;
			if ((utf8Char[1] & 0b1100'0000) != 0b1000'0000)
				throw InvalidCharacterError("Couldn't convert UTF-8 character to unicode character, invalid UTF-8 character (second byte has wrong/no prefix)");
			chr |= (utf8Char[1] & 0b0011'1111) << 6;
			if ((utf8Char[2] & 0b1100'0000) != 0b1000'0000)
				throw InvalidCharacterError("Couldn't convert UTF-8 character to unicode character, invalid UTF-8 character (third byte has wrong/no prefix)");
			chr |= (utf8Char[2] & 0b0011'1111) << 0;
			break;
		case 4:
			chr |= (utf8Char[0] & 0b0000'0111) << 18;
			if ((utf8Char[1] & 0b1100'0000) != 0b1000'0000)
				throw InvalidCharacterError("Couldn't convert UTF-8 character to unicode character, invalid UTF-8 character (second byte has wrong/no prefix)");
			chr |= (utf8Char[1] & 0b0011'1111) << 12;
			if ((utf8Char[2] & 0b1100'0000) != 0b1000'0000)
				throw InvalidCharacterError("Couldn't convert UTF-8 character to unicode character, invalid UTF-8 character (third byte has wrong/no prefix)");
			chr |= (utf8Char[2] & 0b0011'1111) << 6;
			if ((utf8Char[3] & 0b1100'0000) != 0b1000'0000)
				throw InvalidCharacterError("Couldn't convert UTF-8 character to unicode character, invalid UTF-8 character (fourth byte has wrong/no prefix)");
			chr |= (utf8Char[3] & 0b0011'1111) << 0;
			break;
	}

	return chr;
}

size_t Magma::UTF8::FromUnicode(char32_t character, char * utf8Char)
{
	auto size = GetCharSize(character);

	switch (size)
	{
		case 1:
			utf8Char[0] = 0b0000'0000;
			utf8Char[0] |= (character & 0b0111'1111);
			break;
		case 2:
			utf8Char[0] = 0b1100'0000;
			utf8Char[0] |= ((character >> 06) & 0b0001'1111);
			utf8Char[1] = 0b1000'0000;
			utf8Char[1] |= ((character >> 00) & 0b0011'1111);
			break;
		case 3:
			utf8Char[0] = 0b1110'0000;
			utf8Char[0] |= ((character >> 12) & 0b0000'1111);
			utf8Char[1] = 0b1000'0000;
			utf8Char[1] |= ((character >> 06) & 0b0011'1111);
			utf8Char[2] = 0b1000'0000;
			utf8Char[2] |= ((character >> 00) & 0b0011'1111);
			break;
		case 4:
			utf8Char[0] = 0b1111'0000;
			utf8Char[0] |= ((character >> 18) & 0b0000'0111);
			utf8Char[1] = 0b1000'0000;
			utf8Char[1] |= ((character >> 12) & 0b0011'1111);
			utf8Char[2] = 0b1000'0000;
			utf8Char[2] |= ((character >> 06) & 0b0011'1111);
			utf8Char[3] = 0b1000'0000;
			utf8Char[3] |= ((character >> 00) & 0b0011'1111);
			break;
	}

	return size;
}

Magma::String::String()
{
	m_size = 0;
	m_length = 0;
	m_data = (char*)malloc(m_size + 1);
	memset(m_data, '\0', m_size + 1);
}

Magma::String::String(const char * utf8CString)
{
	m_data = nullptr;
	if (!UTF8::IsValid(utf8CString))
		throw InvalidStringError("Failed to construct String from UTF-8 C string, invalid UTF-8");

	m_size = strlen(utf8CString);
	m_data = (char*)malloc(m_size + 1);
	memset(m_data, '\0', m_size + 1);
	memcpy(m_data, utf8CString, m_size);

	m_length = UTF8::GetStringLength(m_data);
}

Magma::String::String(const String & string)
{
	m_size = string.m_size;
	m_length = string.m_length;
	m_data = (char*)malloc(m_size + 1);
	memset(m_data, '\0', m_size + 1);
	memcpy(m_data, string.m_data, m_size);
}

Magma::String::String(String && string) noexcept
{
	m_data = string.m_data;
	m_size = string.m_size;
	m_length = string.m_length;

	string.m_data = nullptr;
	string.m_size = 0;
	string.m_length = 0;
}

Magma::String & Magma::String::operator=(const char * utf8CString)
{
	if (!UTF8::IsValid(utf8CString))
		throw InvalidStringError("Failed to copy UTF-8 C string to String, invalid UTF-8");

	m_size = strlen(utf8CString);
	m_data = (char*)realloc(m_data, m_size + 1);
	memset(m_data, '\0', m_size + 1);
	memcpy(m_data, utf8CString, m_size);

	m_length = UTF8::GetStringLength(m_data);

	return *this;
}

Magma::String & Magma::String::operator=(const String & string)
{
	m_size = string.m_size;
	m_length = string.m_length;
	m_data = (char*)realloc(m_data, string.m_size + 1);
	memset(m_data, '\0', m_size + 1);
	memcpy(m_data, string.m_data, m_size);

	return *this;
}

Magma::String & Magma::String::operator=(String && string) noexcept
{
	if (this != &string)
	{
		if (m_data != nullptr)
		{
			free(m_data);
			m_data = nullptr;
		}

		m_size = string.m_size;
		m_length = string.m_length;
		m_data = string.m_data;

		string.m_size = 0;
		string.m_length = 0;
		string.m_data = nullptr;
	}

	return *this;
}

void Magma::String::AppendChar(char32_t character)
{
	auto size = UTF8::GetCharSize(character);
	m_size = m_size + size;
	++m_length;
	m_data = (char*)realloc(m_data, m_size + 1);
	UTF8::FromUnicode(character, m_data + (m_size - size));
	m_data[m_size] = '\0';
}

size_t Magma::String::AppendChar(const char * character)
{
	auto size = UTF8::GetCharSize(character);
	m_size = m_size + size;
	++m_length;
	m_data = (char*)realloc(m_data, m_size + 1);
	memcpy(m_data + (m_size - size), character, size);
	m_data[m_size] = '\0';

	return size;
}

void Magma::String::AppendString(const char * utf8CString)
{
	if (!UTF8::IsValid(utf8CString))
		throw InvalidStringError("Failed to append UTF-8 C string to String, invalid UTF-8");

	auto size = strlen(utf8CString);
	m_size = m_size + size;
	m_data = (char*)realloc(m_data, m_size + 1);
	memcpy(m_data + (m_size - size), utf8CString, size);
	m_data[m_size] = '\0';

	m_length = 0;
	for (size_t i = 0; i < m_size;)
	{
		size_t size = UTF8::GetCharSize(&m_data[i]);
		i += size;
		++m_length;
	}
}

void Magma::String::AppendString(const String & string)
{
	m_size = m_size + string.m_size;
	m_length = m_length + string.m_length;
	m_data = (char*)realloc(m_data, m_size + 1);
	memcpy(m_data + (m_size - string.m_size), string.CString(), string.m_size);
	m_data[m_size] = '\0';
}

void Magma::String::Pop()
{
	if (m_length == 0)
		throw std::out_of_range("Failed to pop character from string, string is already empty");

	size_t index = 0;
	for (size_t i = 0; i < m_length - 1; ++i)
		index += UTF8::GetCharSize(&m_data[index]);
	m_size -= UTF8::GetCharSize(&m_data[index]);
	--m_length;

	m_data = (char*)realloc(m_data, m_size + 1);
	m_data[m_size] = '\0';
}

void Magma::String::Clear()
{
	m_size = 0;
	m_length = 0;
	m_data = (char*)realloc(m_data, 1);
	m_data[0] = '\0';
}

Magma::String Magma::String::operator+(const String & rhs) const
{
	String str = *this;
	str.AppendString(rhs);
	return std::move(str);
}

Magma::String & Magma::String::operator+=(const String & rhs)
{
	this->AppendString(rhs);
	return *this;
}

Magma::String Magma::String::operator+(const char * rhs) const
{
	String str = *this;
	str.AppendString(rhs);
	return std::move(str);
}

Magma::String & Magma::String::operator+=(const char * rhs)
{
	this->AppendString(rhs);
	return *this;
}

Magma::String Magma::String::Substring(size_t index, size_t length) const
{
	if (index + length > m_length)
		throw std::out_of_range("Failed to get substring from string, substring index + substring length > string length");
	String substring;
	size_t realIndex = 0;
	for (size_t i = 0; i < index; ++i)
		realIndex += UTF8::GetCharSize(&m_data[realIndex]);
	for (size_t i = 0; i < length; ++i)
	{
		substring.AppendChar(&m_data[realIndex]);
		realIndex += UTF8::GetCharSize(&m_data[realIndex]);
	}
	return std::move(substring);
}

std::vector<Magma::String> Magma::String::Split(char32_t delimeter) const
{
	std::vector<String> substrings;
	size_t startIndex = 0, length = 0;
	for (size_t i = 0; i < m_length; ++i)
	{
		if (this->At(i) == delimeter)
		{
			substrings.push_back(std::move(this->Substring(startIndex, length)));
			startIndex = i + 1;
			length = 0;
		}
		else ++length;
	}
	if (length > 0)
		substrings.push_back(std::move(this->Substring(startIndex, length)));
	return std::move(substrings);
}

std::vector<Magma::String> Magma::String::Split(const String & delimeter) const
{
	if (delimeter.Empty())
		return std::move(std::vector<Magma::String> { *this });

	std::vector<String> substrings;
	size_t startIndex = 0, length = 0;
	size_t charsFound = 0;
	for (size_t i = 0; i < m_length; ++i)
	{
		if (this->At(i) == delimeter.At(charsFound))
		{
			++charsFound;
			if (charsFound == delimeter.Length())
			{
				charsFound = 0;
				substrings.push_back(std::move(this->Substring(startIndex, length)));
				startIndex = i + 1;
				length = 0;
			}
		}
		else
		{
			++length;
			if (charsFound > 0)
			{
				length += charsFound;
				charsFound = 0;
			}
		}
	}
	if (length > 0)
		substrings.push_back(std::move(this->Substring(startIndex, length)));
	return std::move(substrings);
}

char32_t Magma::String::At(size_t index) const
{
	if (index >= m_length)
		throw std::out_of_range("Failed to get character from string, index out of range");

	size_t realIndex = 0;
	for (size_t i = 0; i < index; ++i)
		realIndex += UTF8::GetCharSize(&m_data[realIndex]);
	return UTF8::ToUnicode(&m_data[realIndex]);
}

void Magma::String::Set(size_t index, char32_t character)
{
	if (index >= m_length)
		throw std::out_of_range("Failed to set character in string, index out of range");

	size_t realIndex = 0;
	for (size_t i = 0; i < index; ++i)
		realIndex += UTF8::GetCharSize(&m_data[realIndex]);
	size_t oldChrSize = UTF8::GetCharSize(&m_data[realIndex]);
	size_t newChrSize = UTF8::GetCharSize(character);

	if (oldChrSize == newChrSize)
		UTF8::FromUnicode(character, &m_data[realIndex]);
	else
	{
		size_t oldSize = m_size;

		if (newChrSize < oldChrSize)
			m_size = m_size - (oldChrSize - newChrSize);
		else if (newChrSize > oldChrSize)
			m_size = m_size + (newChrSize - oldChrSize);

		// Create new buffer with new data
		char* newData = (char*)malloc(m_size + 1);
		memcpy(newData, m_data, realIndex);
		memcpy(newData + realIndex + newChrSize, m_data + realIndex + oldChrSize, oldSize - realIndex - oldChrSize);
		UTF8::FromUnicode(character, &newData[realIndex]);
		newData[m_size] = '\0';

		// Switch old buffer with new buffer
		free(m_data);
		m_data = newData;
	}
}

void Magma::String::Erase(size_t index)
{
	if (index >= m_length)
		throw std::out_of_range("Failed to erase character in string, index out of range");

	size_t realIndex = 0;
	for (size_t i = 0; i < index; ++i)
		realIndex += UTF8::GetCharSize(&m_data[realIndex]);
	size_t oldChrSize = UTF8::GetCharSize(&m_data[realIndex]);

	size_t oldSize = m_size;
	m_size -= oldChrSize;

	// Create new buffer with new data
	char* newData = (char*)malloc(m_size + 1);
	memcpy(newData, m_data, realIndex);
	memcpy(newData + realIndex, m_data + realIndex + oldChrSize, oldSize - realIndex - oldChrSize);
	newData[m_size] = '\0';

	// Switch old buffer with new buffer
	free(m_data);
	m_data = newData;
}

void Magma::String::Erase(size_t index, size_t length)
{
	if (index + length > m_length)
		throw std::out_of_range("Failed to erase substring in string, substring index + substring length > string length");
	m_length -= length;

	size_t realIndex = 0;
	for (size_t i = 0; i < index; ++i)
		realIndex += UTF8::GetCharSize(&m_data[realIndex]);

	size_t size = 0;
	for (size_t l = 0; l < length; ++l)
		size += UTF8::GetCharSize(&m_data[realIndex + size]);

	size_t oldSize = m_size;
	m_size -= size;

	// Create new buffer with new data
	char* newData = (char*)malloc(m_size + 1);
	memcpy(newData, m_data, realIndex);
	memcpy(newData + realIndex, m_data + realIndex + size, oldSize - realIndex - size);
	newData[m_size] = '\0';

	// Switch old buffer with new buffer
	free(m_data);
	m_data = newData;
}

void Magma::String::Insert(size_t index, char32_t character)
{
	if (index > m_length)
		throw std::out_of_range("Failed to insert character in string, index out of range");
	++m_length;

	size_t realIndex = 0;
	for (size_t i = 0; i < index; ++i)
		realIndex += UTF8::GetCharSize(&m_data[realIndex]);
	size_t chrSize = UTF8::GetCharSize(character);

	size_t oldSize = m_size;
	m_size += chrSize;

	// Create new buffer with new data
	char* newData = (char*)malloc(m_size + 1);
	memcpy(newData, m_data, realIndex);
	memcpy(newData + realIndex + chrSize, m_data + realIndex, oldSize - realIndex);
	UTF8::FromUnicode(character, &newData[realIndex]);
	newData[m_size] = '\0';

	// Switch old buffer with new buffer
	free(m_data);
	m_data = newData;
}

void Magma::String::Insert(size_t index, const String & substring)
{
	if (index > m_length)
		throw std::out_of_range("Failed to insert substring in string, index out of range");
	m_length += substring.Length();

	size_t realIndex = 0;
	for (size_t i = 0; i < index; ++i)
		realIndex += UTF8::GetCharSize(&m_data[realIndex]);

	size_t oldSize = m_size;
	m_size += substring.Size();

	// Create new buffer with new data
	char* newData = (char*)malloc(m_size + 1);
	memcpy(newData, m_data, realIndex);
	memcpy(newData + realIndex + substring.Size(), m_data + realIndex, oldSize - realIndex);
	memcpy(&newData[realIndex], substring.CString(), substring.Size());
	newData[m_size] = '\0';

	// Switch old buffer with new buffer
	free(m_data);
	m_data = newData;
}

Magma::String::~String() noexcept
{
	m_size = 0;
	m_length = 0;
	if (m_data != nullptr)
	{
		free(m_data);
		m_data = nullptr;
	}
}
