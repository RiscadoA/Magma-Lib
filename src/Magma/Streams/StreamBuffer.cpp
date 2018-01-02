#include "StreamBuffer.hpp"

#include "..\String.hpp"

Magma::StreamBuffer::StreamBuffer(char32_t * buffer, size_t bufferSize)
{
	this->SetBuffer(buffer, bufferSize);
}

bool Magma::StreamBuffer::PutChar(char32_t character)
{
	if (m_setPtr <= m_bufferEnd && m_setPtr >= m_buffer)
		*m_setPtr = character;
	++m_setPtr;

	if (m_setPtr > m_bufferEnd)
		if (!this->Sync())
			return false;

	return true;
}

char32_t Magma::StreamBuffer::GetChar()
{
	auto chr = this->PeekChar();
	if (chr == StreamBuffer::EndOfFile)
		return StreamBuffer::EndOfFile;
	this->Next();
	return chr;
}

char32_t Magma::StreamBuffer::PeekChar()
{
	while (m_getEndPtr <= m_getPtr)
	{
		auto chr = this->Underflow();
		if (chr == StreamBuffer::EndOfFile)
			return StreamBuffer::EndOfFile;
		*m_getEndPtr = chr;
		++m_getEndPtr;
	}

	return *m_getPtr;
}

void Magma::StreamBuffer::Next()
{
	++m_getPtr;
	if (m_getPtr > m_bufferEnd)
	{
		m_getPtr = m_buffer;
		m_getEndPtr = m_buffer;
	}
}

bool Magma::StreamBuffer::Sync()
{
	auto ptr = m_buffer;
	while (ptr < m_setPtr && ptr <= m_bufferEnd)
	{
		bool ret = this->Overflow(*ptr);
		if (!ret)
			return false;
		++ptr;
	}
	m_setPtr = m_buffer;
	return true;
}

void Magma::StreamBuffer::SetBuffer(char32_t * buffer, size_t bufferSize)
{
	m_buffer = buffer;
	m_bufferSize = bufferSize;
	m_bufferEnd = m_buffer + m_bufferSize - 1;
	m_getPtr = m_buffer;
	m_getEndPtr = m_buffer;
	m_setPtr = m_buffer;
}

bool Magma::StreamBuffer::Overflow(char32_t character)
{
	// Default fails
	return false;
}

char32_t Magma::StreamBuffer::Underflow()
{
	// Default fails
	return StreamBuffer::EndOfFile;
}
