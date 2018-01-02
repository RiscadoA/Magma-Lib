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
	m_data = (char*)realloc(m_data, m_size + 1);
	UTF8::FromUnicode(character, m_data + (m_size - size));
	m_data[m_size] = '\0';
}

size_t Magma::String::AppendChar(const char * character)
{
	auto size = UTF8::GetCharSize(character);
	m_size = m_size + size;
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
}

void Magma::String::AppendString(const String & string)
{
	m_size = m_size + string.m_size;
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

Magma::String Magma::String::operator+(const String & rhs)
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

Magma::String Magma::String::operator+(const char * rhs)
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
