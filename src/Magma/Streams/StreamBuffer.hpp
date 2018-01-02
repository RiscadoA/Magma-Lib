#pragma once

namespace Magma
{
	/// <summary>
	///		Underlying buffer used in streams
	/// </summary>
	class StreamBuffer
	{
	public:
		static const char32_t EndOfFile = -1;

		/// <summary>
		///		Constructs a stream buffer
		/// </summary>
		/// <param name="buffer">Buffer that will be used</param>
		/// <param name="bufferSize">Buffer size</param>
		StreamBuffer(char32_t* buffer, size_t bufferSize);

		/// <summary>
		///		Puts a character in the buffer and increments the set pointer
		/// </summary>
		/// <param name="character">New character</param>
		/// <returns>True if everything went right, otherwise false</returns>
		virtual bool PutChar(char32_t character);

		/// <summary>
		///		Gets the next character in the buffer and increments the get pointer
		/// </summary>
		/// <returns>Next character in the buffer</returns>
		virtual char32_t GetChar();

		/// <summary>
		///		Gets the next character in the buffer
		/// </summary>
		/// <returns>Next character in the buffer</returns>
		virtual char32_t PeekChar();

		/// <summary>
		///		Increments the get pointer
		/// </summary>
		virtual void Next();

		/// <summary>
		///		Syncs this stream buffer buffer with the output character sequence
		/// </summary>
		/// <returns>True if everything went right, otherwise false</returns>
		virtual bool Sync();

		/// <summary>
		///		Sets the buffer to be used on this stream buffer
		/// </summary>
		/// <param name="buffer">Buffer</param>
		/// <param name="bufferSize">Buffer size</param>
		void SetBuffer(char32_t* buffer, size_t bufferSize);
		
		/// <summary>
		///		Gets the buffer being used on this stream buffer
		/// </summary>
		/// <returns>Buffer being used on this stream buffer</returns>
		inline char32_t* GetBuffer() { return m_buffer; }

		/// <summary>
		///		Gets the buffer being used on this stream buffer
		/// </summary>
		/// <returns>Buffer being used on this stream buffer</returns>
		inline const char32_t* GetBuffer() const { return m_buffer; }

	protected:
		/// <summary>
		///		Puts a character in the output character sequence
		/// </summary>
		/// <param name="character">Character unicode point</param>
		/// <returns>True if everything went right, otherwise returns false</returns>
		virtual bool Overflow(char32_t character);

		/// <summary>
		///		Gets a character from the input character sequence
		/// </summary>
		/// <returns>Character unicode point if everything went right, otherwise returns StreamBuffer::EndOfFile</returns>
		virtual char32_t Underflow();

		size_t m_bufferSize;
		char32_t* m_buffer;
		char32_t* m_bufferEnd;
		char32_t* m_getPtr;
		char32_t* m_getEndPtr;
		char32_t* m_setPtr;
	};
}
