#pragma once

#include "IStream.hpp"
#include "OStream.hpp"

namespace Magma
{
	/// <summary>
	///		Redirects stream buffer to stdout
	/// </summary>
	class STDOutStreamBuffer final : public StreamBuffer
	{
	public:
		STDOutStreamBuffer(char32_t* buffer, size_t bufferSize);
		virtual bool Sync() final;
	protected:
		virtual bool Overflow(char32_t character) final;
	};

	/// <summary>
	///		Redirects stream buffer to stdin
	/// </summary>
	class STDInStreamBuffer final : public StreamBuffer
	{
	public:
		STDInStreamBuffer(char32_t* buffer, size_t bufferSize);
	protected:
		virtual char32_t Underflow() final;
	};
}